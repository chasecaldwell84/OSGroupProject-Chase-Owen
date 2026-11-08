/*
#include "Workload 1 - Independent Loop Computation/loop.h"
#include <iostream>

int main() {
    string schedules[] = { STATIC, CYCLIC, DYNAMIC };
    int threadCounts[] = { 1, 2, 3, 4, 7 };

    for (const string& s : schedules) {
        for (int t : threadCounts) {
            //            workload mode        size     threads proc sched chunk seed   reps
            config c = { LOOP,    STD_THREAD, 1000000, t,      1,   s,    1000, 12345, 3 };
            Loop loop(c);
            output r = loop.run();
            cout << s << "  threads=" << t
                 << "  checksum=" << (uint64_t)r.checksum
                 << "  avg_ms=" << r.avg_time_ms
                 << "  valid=" << (r.valid ? "YES" : "NO") << endl;
        }
    }
    return 0;
}

*/