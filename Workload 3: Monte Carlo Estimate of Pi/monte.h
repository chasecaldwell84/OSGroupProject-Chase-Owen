#ifndef MONTE_H
#define MONTE_H
#include "../constants.h"
using namespace std;

/*
    The config Montecarlo will recieve from the main 
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
class MonteCarlo {
public:
    MonteCarlo(const config& config);
    config config;
    output run();

private:

    static void threadWorker(unsigned int seed, unsigned int count, unsigned int* out);
    double sequentialEstimate();
    double threadEstimate();
    double MPIestimate();
    double OPENMPestimate();
};


#endif // MONTE_H
