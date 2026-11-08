#include "constants.h"
#include <sstream>

bool getconfig(config& config, istream& pin) {
    stringstream input;
    input << pin.rdbuf();
    string label;

    input >> label;
    if (label != "WORKLOAD" || !(input >> config.workload)) return false;

    input >> label;
    if (label != "MODE" || !(input >> config.mode)) return false;

    input >> label;
    if (config.mode == TRACE) { // MODEL is only there for TRACE
        if (label != "MODEL" || !(input >> config.model)) return false;
        input >> label;
    }
    if (label != "SIZE" || !(input >> config.size) || config.size <= 0) return false;

    input >> label;
    if (label != "THREADS" || !(input >> config.threads) || config.threads <= 0) return false;

    input >> label;
    if (label != "PROCESSES" || !(input >> config.processes) || config.processes <= 0) return false;

    input >> label;
    if (label != "SCHEDULE" || !(input >> config.schedule)) return false;

    input >> label;
    if (label != "CHUNK" || !(input >> config.chunk) || config.chunk <= 0) return false;

    input >> label;
    if (label != "SEED" || !(input >> config.seed)) return false;

    input >> label;
    if (label != "REPETITIONS" || !(input >> config.repetitions) || config.repetitions <= 0) return false;

    return !(input >> label); 
}

void printconfig(const config& config, ostream& pout) {
    pout << "WORKLOAD: " << config.workload << endl;
    pout << "MODE: " << config.mode << endl;
    pout << "SIZE: " << config.size << endl;
    pout << "THREADS: " << config.threads << endl;
    pout << "PROCESSES: " << config.processes << endl;
    pout << "SCHEDULE: " << config.schedule << endl;
    pout << "CHUNK: " << config.chunk << endl;
    pout << "SEED: " << config.seed << endl;
    pout << "REPETITIONS: " << config.repetitions << endl;
}

void printoutput(const output& output, ostream& pout) {
    pout << "WORKLOAD: " << output.workload << endl;
    pout << "MODE: " << output.mode << endl;
    pout << "SIZE: " << output.size << endl;
    pout << "WORKERS: " << output.workers << endl;
    pout << "SCHEDULE: " << output.schedule << endl;
    pout << "RESULT: " << output.result << endl;
    pout << "CHECKSUM: " << output.checksum << endl;
    pout << "AVG_TIME_MS: " << output.avg_time_ms << endl;
    pout << "BEST_TIME_MS: " << output.best_time_ms << endl;
    pout << "WORST_TIME_MS: " << output.worst_time_ms  << endl;
    pout << "REPETITIONS: " << output.repetitions  << endl;
    if (output.valid) {
        pout << "VALID: YES"  << endl;
    } else {
        pout << "VALID: NO"  << endl;
    }
}
