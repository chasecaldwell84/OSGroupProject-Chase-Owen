#include "loop.h"

double* Loop::sequentialEstimate() {
    double* res;
    res = new double[config.size];

    for (int i = 0; i < config.size; i++) {
        res[i] = (i * i + 3 * i + 7) % this->config.seed;
    }

    return res;
}

double* Loop::threadEstimate() {}

double* Loop::MPIestimate() {}

double* Loop::OPENMPestimate() {}

//constructor
Loop::Loop(const ::config &config) {
    this->config = config;
}

output Loop::run() {
    output res;

    if (this->config.mode == SEQUENTIAL) {
        sequentialEstimate();
    }else if (this->config.mode == STD_THREAD) {
        threadEstimate();
    }else if (this->config.mode == MPI) {
        MPIestimate();
    }else if (this->config.mode == OPENMP) {
        OPENMPestimate();
    }

    return res;
}
