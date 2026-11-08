#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>
#include <iostream>
using namespace std;


/*
Required Input Format
WORKLOAD workload
MODE mode
[MODEL model]
SIZE n
THREADS t
PROCESSES p
SCHEDULE schedule
CHUNK c
SEED s
REPETITIONS r


Example input for the program.
 WORKLOAD DOT
MODE COMPARE
SIZE 10000000
THREADS 4
PROCESSES 4
SCHEDULE STATIC
CHUNK 1000
SEED 12345
REPETITIONS 5
 */
struct config {
    string workload;
    string mode;
    string model;
    int size;
    int threads;
    int processes;
    string schedule;
    int chunk;
    int seed;
    int repetitions;
};


bool getconfig(config& config, istream& pin);

void printconfig(const config& config, ostream& pout);


/*
Required Output Format
WORKLOAD: DOT
MODE: STD_THREAD
SIZE: 10000000
WORKERS: 4
SCHEDULE: STATIC
RESULT: value
CHECKSUM: value
AVG_TIME_MS: value
BEST_TIME_MS: value
WORST_TIME_MS: value
REPETITIONS: r
VALID: YES
*/
struct output{
    string workload = "NONE";
    string mode = "NONE";
    int size = 0;
    int workers = 0;
    string schedule;
    double result = 0.0;
    double checksum = 0.0;
    double avg_time_ms = 0.0;
    double best_time_ms = 0.0;
    double worst_time_ms = 0.0  ;
    int repetitions = 0;
    bool valid = false;

};

void printoutput(const output& output, ostream& pout);

const string LOOP = "LOOP";
const string DOT = "DOT";
const string PI = "PI";
const string SEQUENTIAL = "SEQUENTIAL";
const string STD_THREAD = "STD_THREAD";
const string OPENMP = "OPENMP";
const string MPI = "MPI";
const string COMPARE = "COMPARE";
const string TRACE = "TRACE";
const string STATIC = "STATIC";
const string CYCLIC = "CYCLIC";
const string DYNAMIC = "DYNAMIC";

#endif // CONSTANTS_H
