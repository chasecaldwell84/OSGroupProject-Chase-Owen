#ifndef DOT_H
#define DOT_H
#include "../constants.h"
using namespace std;

/*
    The config DotProduct will recieve from the main
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
class DotProduct {
public:
    DotProduct(const config& config);
    config config;
    output run();

private:

    double sequentialEstimate();
    double threadEstimate();
    double MPIestimate();
    double OPENMPestimate();
};


#endif // DOT_H
