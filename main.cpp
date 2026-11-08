#include "Workload 1: Independent Loop Computation/loop.h"
#include "Workload 2: Dot Product with Reduction/dot.h"
#include "Workload 3: Monte Carlo Estimate of Pi/monte.h"
#include <iostream>
#include <string>
using namespace std;


/*
struct output{
    string workload;
    string mode;
    int size;
    int workers;
    string schedule;
    double result;
    double checksum;
    double avg_time_ms;
    double best_time_ms;
    double worst_time_ms;
    int repetitions;
    bool valid;

};*/
output run(const config& config){
    output result;
    if (config.workload == LOOP) {
        Loop loop(config);
        result = loop.run();
    } else if (config.workload == DOT) {
        Dot dot(config);
        result = dot.run();
    } else if (config.workload == PI) {
        MonteCarlo monte(config);
        result = monte.run();
    }
    result.workload = config.workload;
    result.mode = config.mode;
    result.size = config.size;
    if (config.mode == MPI) {
        result.workers = config.processes;
    } else if (config.mode == SEQUENTIAL) {
        result.workers = 1;
    } else {
        result.workers = config.threads;
    }
    result.schedule = config.schedule;
    result.repetitions = config.repetitions;


    return result;
}





int main() {
    config input;
    output result;
    if (!getconfig(input, cin)) {
        cout << "INVALID_INPUT" << endl;
        return 0;
    }
    printconfig(input, cout);
    result = run(input);
    printoutput(result, cout);

    return 0;
}
