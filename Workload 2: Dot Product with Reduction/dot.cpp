#include "dot.h"
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <omp.h>
#include <stdexcept>

using namespace std;

Dot::Dot(const ::config& config_value) {
    this->config = config_value;
}
/*
double Dot::sequentialEstimate(){
    srand((unsigned int)config.seed);
    unsigned int inside = 0;
    for (unsigned int i = 0; i < config.size; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0) {
            inside++;
        }
    }
    return 4.0 * inside / config.size;
}*/


double Dot::sequentialEstimate() {
    double sum = 0.0;

    for (int i = 0; i < config.size; i++) {
        double a = dotA(i, config.seed);
        double b = dotB(i, config.seed);

        sum += a * b;
    }

    return sum;
}


double Dot::threadEstimate(){
    return 0.0;
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

    if (config.mode == SEQUENTIAL || config.mode == OPENMP) {
        double expected = sequentialEstimate();
        result.valid = (result.result == expected);
        result.checksum = result.result;
    }
    return result;
}
