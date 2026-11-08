#ifndef DOT_H
#define DOT_H
#include "../constants.h"
#include <cstdint>
#include <vector>
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



//values shared by all DOT implementations
inline double dotA(int i, int seed) {
    uint64_t x = static_cast<uint32_t>(seed);
    x += static_cast<uint64_t>(i) * 17ULL;
    return static_cast<double>((x % 10ULL) + 1ULL);
}

inline double dotB(int i, int seed) {
    uint64_t x = static_cast<uint32_t>(seed);
    x += static_cast<uint64_t>(i) * 31ULL + 7ULL;
    return static_cast<double>((x % 10ULL) + 1ULL);
}

class Dot {
public:
    Dot(const config& config);
    config config;
    output run();

private:

    double sequentialEstimate();
    double threadEstimate();
    double MPIestimate();
    double OPENMPestimate();
};


#endif // DOT_H
