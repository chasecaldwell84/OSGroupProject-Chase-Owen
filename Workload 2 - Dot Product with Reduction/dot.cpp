#include "dot.h"
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <atomic>
#include <thread>
#include <omp.h>
#include <stdexcept>

using namespace std;

Dot::Dot(const ::config& config_value) {
    this->config = config_value;
}

double Dot::sequentialEstimate() {
    double sum = 0.0;

    for (int i = 0; i < config.size; i++) {
        double a = dotA(i, config.seed);
        double b = dotB(i, config.seed);

        sum += a * b;
    }

    return sum;
}
//calculates sum for specified index
long Dot::rangeSum(long start, long end) {
    long sum = 0;
    for (long i = start; i < end; i++) {
        double a = dotA(i, config.seed);
        double b = dotB(i, config.seed);
        sum += a * b;
    }
    return sum;
}

void Dot::threadWorker(int id, atomic<long>* next, long* slot) {
    long size = config.size;
    long chunk = config.chunk;
    long threads = config.threads;
    long sum = 0;
    long start = 0;
    long end = 0;

    if (config.schedule == STATIC) {
        long split = size / threads;
        long remainder = size % threads;
        start = split * id;
        if (id < remainder) {
            start += id;
        }
        else {
            start += remainder;
        }
        end = start + split;
        if (id < remainder) {
            end += 1;
        }
        sum = rangeSum(start, end);
    }
    else if (config.schedule == CYCLIC) {
        vector<long> idxs;
        start = id * chunk;
        while (start < size) {
            end = start + min(size - start, chunk);
            idxs.push_back(start);
            idxs.push_back(end);
            start += chunk * threads;
        }
        for (long i = 0; i < idxs.size(); i+=2) {
            sum += rangeSum(idxs[i], idxs[i+1]);
        }
    }
    else if (config.schedule == DYNAMIC) {
        while (true) {
            start = next->fetch_add(chunk);
            if (start >= size) {break;}
            end = start + min(size - start, chunk);
            sum += rangeSum(start, end);
        }
    }
    else {
        throw std::invalid_argument("Invalid schedule");
    }

    *slot = sum;
}

double Dot::threadEstimate(){
    unsigned int t = config.threads;
    unsigned int s = config.size / t;
    unsigned int r = config.size % t;
    vector<long> results(t, 0);
    vector<thread> workers;
    atomic<long> next(0);

    for (int i = 0; i < t; i++) {
        workers.push_back(thread(&Dot::threadWorker, this, i,  &next, &results[i]));
        }
        for (int i = 0; i < t; i++) {
            workers[i].join();
        }
        long ans = 0;
        for (long result : results) {
           ans += result;
        }
        return ans;
}
double Dot::MPIestimate(){
    return 0.0;
}

double Dot::OPENMPestimate() {
    double sum = 0.0;

    // Configure the requested OpenMP schedule.
    if (config.schedule == STATIC) {
        omp_set_schedule(omp_sched_static, 0);
    }
    else if (config.schedule == CYCLIC) {
        omp_set_schedule(omp_sched_static, config.chunk);
    }
    else if (config.schedule == DYNAMIC) {
        omp_set_schedule(omp_sched_dynamic, config.chunk);
    }
    else {
        throw std::invalid_argument("Invalid schedule");
    }

    omp_set_dynamic(0);

    #pragma omp parallel for \
        num_threads(config.threads) \
        schedule(runtime) reduction(+:sum)
    for (int i = 0; i < config.size; i++) {
        double a = dotA(i, config.seed);
        double b = dotB(i, config.seed);

        sum += a * b;
    }

    return sum;
}



output Dot::run() {
    output result;
    double total_time = 0.0;

    for (int rep = 0; rep < config.repetitions; rep++) {
        auto start = chrono::steady_clock::now();
        if (config.mode == SEQUENTIAL) {
            result.result = sequentialEstimate();
        } else if (config.mode == STD_THREAD) {
            result.result = threadEstimate();
        } else if (config.mode == MPI) {
            result.result = MPIestimate();
        } else if (config.mode == OPENMP) {
            result.result = OPENMPestimate();
        }
        auto end = chrono::steady_clock::now();

        double ms = chrono::duration<double, milli>(end - start).count();
        total_time += ms;
        if (rep == 0 || ms < result.best_time_ms) {
            result.best_time_ms = ms;
        }
        if (rep == 0 || ms > result.worst_time_ms) {
            result.worst_time_ms = ms;
        }
    }

    if (config.repetitions > 0) {
        result.avg_time_ms = total_time / config.repetitions;
    }

    result.workload = DOT;
    result.mode = config.mode;
    result.size = config.size;
    result.schedule = config.schedule;
    result.repetitions = config.repetitions;

    result.workers = (config.mode == SEQUENTIAL) ? 1 : config.threads;

    if (config.mode == SEQUENTIAL || config.mode == OPENMP || config.mode == STD_THREAD) {
        double expected = sequentialEstimate();
        result.valid = (result.result == expected);
        result.checksum = result.result;
    }
    return result;
}
