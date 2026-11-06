#include "monte.h"
#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <vector>
#include <omp.h>

using namespace std;

MonteCarlo::MonteCarlo(const ::config& config_value) {
    this->config = config_value;
}

double MonteCarlo::sequentialEstimate(){
    mt19937 gen((unsigned int)config.seed);
    uniform_real_distribution<double> dist(0.0, 1.0);
    unsigned int inside = 0;
    for (unsigned int i = 0; i < config.size; i++) {
        double x = dist(gen);
        double y = dist(gen);
        if (x * x + y * y <= 1.0) {
            inside++;
        }
    }
    return 4.0 * inside / config.size;
}

void MonteCarlo::threadWorker(unsigned int seed, unsigned int count, unsigned int* out){
    mt19937 gen(seed);
    uniform_real_distribution<double> dist(0.0, 1.0);
    unsigned int local = 0;
    for (unsigned int i = 0; i < count; i++) {
        double x = dist(gen);
        double y = dist(gen);
        if (x * x + y * y <= 1.0) {
            local++;
        }
    }
    // CRITICAL SECTION!!!!!!!! :)
    
    *out = local;
}

double MonteCarlo::threadEstimate(){
    int t = config.threads;
    vector<unsigned int> inside(t, 0);
    vector<thread> workers;

    for (int id = 0; id < t; id++) {
        unsigned int count = config.size / t;
        if ((unsigned int)id < config.size % t) {
            count++;
        }
        workers.push_back(thread(&MonteCarlo::threadWorker, (unsigned int)config.seed + id, count, &inside[id]));
    }
    for (int id = 0; id < t; id++) {
        workers[id].join();
    }
    unsigned int total = 0;
    for (int id = 0; id < t; id++) {
        total += inside[id];
    }
    return 4.0 * total / config.size;
}

double MonteCarlo::MPIestimate(){
    return 0.0;
}
double MonteCarlo::OPENMPestimate(){
    unsigned int inside = 0;
    #pragma omp parallel num_threads(config.threads) reduction(+:inside)
    {
        mt19937 gen((unsigned int)config.seed + omp_get_thread_num());
        uniform_real_distribution<double> dist(0.0, 1.0);
        #pragma omp for
        for (int i = 0; i < config.size; i++) {
            double x = dist(gen);
            double y = dist(gen);
            if (x * x + y * y <= 1.0) {
                inside++;
            }
        }
    }
    return 4.0 * inside / config.size;
}


output MonteCarlo::run() {
    output result;
    double total_time = 0.0;

    for (int rep = 0; rep < config.repetitions; rep++) {
        chrono::steady_clock::time_point start = chrono::steady_clock::now();
        if (config.mode == SEQUENTIAL) {
            result.result = sequentialEstimate();
        } else if (config.mode == STD_THREAD) {
            result.result = threadEstimate();
        } else if (config.mode == MPI) {
            result.result = MPIestimate();
        } else if (config.mode == OPENMP) {
            result.result = OPENMPestimate();
        }
        chrono::steady_clock::time_point end = chrono::steady_clock::now();

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
    return result;
}
