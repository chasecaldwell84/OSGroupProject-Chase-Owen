#include "dot.h"
#include <iostream>
#include <cstdlib>

using namespace std;

Dot::Dot(const ::config& config_value) {
    this->config = config_value;
}

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
}

double Dot::threadEstimate(){
    return 0.0;
}
double Dot::MPIestimate(){
    return 0.0;
}
double Dot::OPENMPestimate(){
    return 0.0;
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
    return result;
}
