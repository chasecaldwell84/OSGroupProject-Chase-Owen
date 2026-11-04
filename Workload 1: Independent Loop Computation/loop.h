#ifndef LOOP_H
#define LOOP_H
#include "../constants.h"
using namespace std;

/*
    The config Loop will recieve from the main
    struct config {
    string workload;
    string mode;
    int size;
    int threads;
    int processes;
    string schedule;
    int chunk;
    int seed;
    int repetitions;
};
*/
class Loop {
public:
    Loop(const config& config);
    config config;
    output run();

private:

    double* sequentialEstimate();
    double* threadEstimate();
    double* MPIestimate();
    double* OPENMPestimate();
};


#endif // LOOP_H
