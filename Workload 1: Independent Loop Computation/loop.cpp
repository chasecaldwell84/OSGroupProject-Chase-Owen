#include "loop.h"
#include <thread>
#include <atomic>
#include <chrono>
#include <algorithm>

// ---------------------------------------------------------------
// formula and helpers
// ---------------------------------------------------------------

// TODO: confirm what the M value should be / Is booth going to give us one?
static const uint64_t M = 1000003;

// One index of the workload
// we're using uint64_t instead of double so i*i can't overflow
static uint64_t loopValue(uint64_t i) {
    return (i * i + 3 * i + 7) % M;
}

// Fills data[start, end) and returns the sum of those values
// Every thread calls this on a different range, so no two threads
// ever write the same element, which is why no mutex is needed
static uint64_t fillRange(vector<uint64_t>& data, uint64_t start, uint64_t end) {
    uint64_t sum = 0;
    for (uint64_t i = start; i < end; i++) {
        data[i] = loopValue(i);
        sum += data[i];
    }
    return sum;
}

// ---------------------------------------------------------------
// SEQUENTIAL (Jack)
// ---------------------------------------------------------------
uint64_t Loop::sequentialEstimate(vector<uint64_t>& data) {
    return fillRange(data, 0, data.size());
}

// ---------------------------------------------------------------
// STD_THREAD (Jonah)
// ---------------------------------------------------------------
uint64_t Loop::threadEstimate(vector<uint64_t>& data) {
    const uint64_t n     = data.size();
    const uint64_t t     = (uint64_t)cfg.threads;
    const uint64_t chunk = (uint64_t)cfg.chunk;

    // one slot per thread and each thread will write to its own slot only
    // once at the end, so there's no race condition
    vector<uint64_t> partial(t, 0);

    // Shared counter for dynamic
    // making it atomic so no two threads can grab the same chunk
    atomic<uint64_t> nextChunk(0);

    vector<thread> workers;
    workers.reserve(t);

    for (uint64_t k = 0; k < t; k++) {
        // [&, k] we're capturing everything by reference except for k
        // k can't be referenced because each thread needs to have its own k
        workers.emplace_back([&, k]() {
            uint64_t sum = 0; //local sum for this thread

            if (cfg.schedule == STATIC) {
                // one block per thread, last block picks up leftovers
                uint64_t block = (n + t - 1) / t;
                uint64_t start = min(n, k * block);
                uint64_t end = min(n, start + block);
                sum = fillRange(data, start, end);
            }
            else if (cfg.schedule == CYCLIC) {
                // chunks dealt in a round robin
                // k, k+t, k+2t, ...
                for (uint64_t start = k * chunk; start < n; start += t * chunk) {
                    sum += fillRange(data, start, min(n, start + chunk));
                }
            }
            else if (cfg.schedule == DYNAMIC) {
                // grav next free chunk until none are left
                // fetch_add returns old val and increments by chunk
                while (true) {
                    uint64_t start = nextChunk.fetch_add(chunk);
                    if (start >= n) break;
                    sum += fillRange(data, start, min(n, start + chunk));
                }
            }

            partial[k] = sum; // write to threads slot
        });
    }

    // Wait for all threads to finish before reading partial
    for (auto& w : workers) {
        w.join();
    }

    // Combine
    uint64_t total = 0;
    for (uint64_t p : partial) {
        total += p;
    }
    return total;
}

/// TODO: OpenMP and MPI implementations
uint64_t Loop::MPIestimate(vector<uint64_t>& data) {return 0;} // Jack
uint64_t Loop::OPENMPestimate(vector<uint64_t>& data) {return 0;} // Jonah

// ---------------------------------------------------------------
// Constructor and run()
// ---------------------------------------------------------------

Loop::Loop(const config &c) : cfg(c) {}

output Loop::run() {
    output res;

    // input checks
    if (cfg.size < 0 || cfg.threads < 1 || cfg.chunk < 1 || cfg.repetitions < 1) {
        res.valid = false;
        return res;
    }

    // Allocated once, outside the timed region
    vector<uint64_t> data(cfg.size);

    // correctness baseline is sequential checksum
    uint64_t expected = sequentialEstimate(data);

    uint64_t checksum = 0;
    double total = 0.0;
    double best = 0.0;
    double worst = 0.0;

    for (int r = 0; r < cfg.repetitions; r++) {
        auto t0 = chrono::steady_clock::now();

        if      (cfg.mode == SEQUENTIAL) checksum = sequentialEstimate(data);
        else if (cfg.mode == STD_THREAD) checksum = threadEstimate(data);
        else if (cfg.mode == OPENMP)     checksum = OPENMPestimate(data);
        else if (cfg.mode == MPI)        checksum = MPIestimate(data);

        auto t1 = chrono::steady_clock::now();
        double ms = chrono::duration<double, milli>(t1 - t0).count();

        total += ms;
        if (r == 0 || ms < best)  best  = ms;
        if (r == 0 || ms > worst) worst = ms;
    }

    res.checksum      = (double)checksum;   
    res.result        = (double)checksum;
    res.avg_time_ms   = total / cfg.repetitions;
    res.best_time_ms  = best;
    res.worst_time_ms = worst;
    res.valid         = (checksum == expected);
    return res;
}
