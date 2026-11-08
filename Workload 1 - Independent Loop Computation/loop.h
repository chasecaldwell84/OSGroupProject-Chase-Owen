#ifndef LOOP_H
#define LOOP_H
#include "../constants.h"
#include <vector>
#include <cstdint>
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
    Loop(const config& c);
    config cfg;
    output run();

private:

    // Each one fills data and returns the checksum
    // data is allocated once in run() so allocation isn't timed
    // using uint64_t instead of a double to avoid overflow issues
    uint64_t sequentialEstimate(vector<uint64_t>& data);
    uint64_t threadEstimate(vector<uint64_t>& data);
    uint64_t OPENMPestimate(vector<uint64_t>& data);
    uint64_t MPIestimate(vector<uint64_t>& data);
};


#endif // LOOP_H
