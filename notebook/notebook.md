*Table or Booth Team Notebook

**Notebook entry template

Date 
Author 
Milestone 
| Type | Meeting / Design decision / Sketch / Diagram / Pseudocode / Race condition / Bug / Fix / Test (pass or fail) / Timing result / Timing anomaly / Milestone notes / Contribution |

**What happened or what we did**

**Details** 

- Attendees if its meeting
- Options considered and the decision
- Sketch, diagram, or pseudocode 
- Steps to reproduce, expected vs actual for bugs, race conditions, failed tests
- What we changed and whether it worked
- Setup and input siz + any other relevant data if tests were run

**Result or outcome**



**First team meeting

Date 10/16/2026
Author Dakota Hernandez
Milestone 
| Type | Meeting|

**What happened or what we did**
We met and split up the work for the team along with making github.

**Details** 

- Attendees: Dakota Hernandez, Jonah Beck, Jackfotenot

-Used when2meet.com to find when the team is all availible to meet in person

![when2meet availability](images/when2meet.png)

**Result or outcome**
| Workload | Owners |
|----------|--------|
| Workload 1: Independent Loop (LOOP) | Jack and Jonah |
| Workload 2: Dot Product (DOT) | Chase and Owen |
| Workload 3: Monte Carlo Pi (PI) | Dakota |


**Milestone 1 Module design diagram

Date 10/21/2026
Author Dakota Hernandez
Milestone 1: Design Proposal
| Type | Diagram |

**What happened
Added the proposed class/module design diagram to the Milestone 1 design proposal.

**Details** 

- Diagram of the module design  section of Milestone 1

![module design](../Milestone%201:%20Design%20Proposal%09/images/moduledesign.png)

**Milestone 1 design proposal completed

Date 10/22/2026
Author Dakota Hernandez
Milestone 1: Design Proposal
| Type | Design decision / Milestone notes |

**What happened or what we did**
Filled in the my Milestone 1 sections: 

**Details** 

**Result or outcome**
Milestone 1 document is ready for the team to finish before submission.


**Milestone 1 Module design diagram update

Date 10/23/2026
Author Dakota Hernandez
Milestone 1: Design Proposal
| Type | Diagram |

**What happened
Added the proposed class/module design diagram to the Milestone 1 design proposal, 
after realizes the previous method of main handling timing was flawed due to 
setup of processes varying

**Details** 

- Diagram of the module design  section of Milestone 1

![updated module design](../Milestone%201:%20Design%20Proposal%09/images/updatedmoduledesign.png)


**Workload files, shared config, and Monte Carlo skeleton

Date 10/26/2026
Author Dakota Hernandez
Milestone 2: Sequential and Shared-Memory Baselines
| Type | Design decision / Contribution |

**What happened or what we did**
Set up the code structure so every workload can be called from main the same way

**Details** 

- Added a folder, header, and source file for each workload (loop, dot, monte).
- Added the config struct and getconfig / printconfig in main so every workload
  reads the same input fields (WORKLOAD, MODE, SIZE, THREADS, PROCESSES, SCHEDULE,
  CHUNK, SEED, REPETITIONS).
- Started the MonteCarlo class. It takes the config so then each workload 
gets the whole config instead of separate arguments, so all
  workloads and models are called the same way.

**Result or outcome**
The structure is done. The models aren't written yet.


**Shared constants.h and run in main

Date 10/27/2026
Author Dakota Hernandez
Milestone 2: Sequential and Shared-Memory Baselines
| Type | Design decision / Bug / Fix / Contribution |

**What happened or what we did**
Moved the shared structs into contants.h and finished run so main runs the
selected workload, I had constants in a parent folder on accident.

**Details** 

- Moved config, output, getconfig, printconfig, and printoutput into
  constants.h in the repo folder so all workloads share them.
- Added constants for the workloads (LOOP, DOT, PI), the modes (SEQUENTIAL, STD_THREAD,
  OPENMP, MPI, COMPARE, TRACE), and the schedules (STATIC, CYCLIC, DYNAMIC).
- run in main calls MonteCarlo(config).run() when WORKLOAD is PI, then fills in the
  fields every workload reports the same way (workload, mode, size, workers, schedule,
  repetitions). main prints the result with printoutput.
-MonteCarlo picks the model from MODE, runs it REPETITIONS times, and times
  each run with chrono to report the average, best, and worst time
  in ms.
- Bug: the build failed with duplicate symbols for getconfig, printconfig, and
  printoutput, because both main.cpp and monte.cpp include constants.h, which
  defines those functions.
- Fix: marked the three functions inline so it would build.

**Result or outcome**
A PI run prints the config and a full report. RESULT is 0.0 until the model functions
are written.


**Shared headers, constants.cpp, and include guards

Date 10/28/2026
Author Dakota Hernandez
Milestone 2: Sequential and Shared-Memory Baselines
| Type | Bug / Fix / Design decision |

**What happened or what we did**
Gave loop.h and dot.h the same baseline as monte.h so all three workloads are
called the same way, and fixed the duplicate symbol bug from 10/27 properly.

**Details** 

- loop.h and dot.h now follow the same layout as monte.h (a class that takes the
  config and has a run method).
- Bug from 10/27: the duplicate symbols for getconfig, printconfig, and printoutput
  were only hidden by marking them inline.
- Fix: moved the definitions into constants.cpp and left only the declarations in
  constants.h, so the inline is gone. Added include guards to constants.h and added
  constants.cpp to CMakeLists.txt.
- main now calls the other workloads (LOOP and DOT) as well as PI.

**Result or outcome**
Builds without inline. 
**Monte Carlo sequential baseline

Date 10/30/2026
Author Dakota Hernandez
Milestone 2: Sequential and Shared-Memory Baselines
| Type | Contribution |

**What happened or what we did**
Wrote sequentialEstimate for the Monte Carlo workload, the baseline the other models
get compared against.

**Details** 

- Seeds the generator with SEED, then for SIZE samples picks a random x and y
  between 0 and 1 and counts the points where x*x + y*y <= 1.
- Estimate of pi = 4 * inside / SIZE.

**Result or outcome**
SEQUENTIAL mode returns an estimate of pi and the timings.


**Switched to mt19937

Date 11/02/2026
Author Dakota Hernandez
Milestone 2: Sequential and Shared-Memory Baselines
| Type | Design decision / Fix |

**What happened or what we did**
Replaced rand with std::mt19937 in the sequential version before writing the thread
version.

**Details** 

- rand keeps hidden global state, so threads would share one sequence and the
  results would not be reproducible.
- mt19937 is not thread safe either if one generator is shared, but each thread can
  own its own, seeded with SEED + thread id.
- The sequential version now creates a local mt19937 and a uniform_real_distribution
  between 0 and 1, this also removes the need for dividing by max size :)

**Result or outcome**
Decision: every model uses mt19937 with its own generator per worker. The estimate
for a given seed changes from the rand version.


**Monte Carlo std::thread implementation

Date 11/04/2026
Author Dakota Hernandez
Milestone 2: Sequential and Shared-Memory Baselines
| Type | Design decision / Contribution / Test (pass) |

**What happened or what we did**
Wrote threadEstimate for the Monte Carlo workload.

**Details** 

- SIZE is split into THREADS blocks. The first SIZE % THREADS threads get one extra
  sample.
- Each thread runs threadWorker with its own mt19937 seeded with SEED + id and counts
  its own hits.
- Critical section: each thread writes only its own slot of the inside vector, so no
  two threads touch the same element and no lock is needed. The main thread adds the
  slots after join, so there is no race.


**Result or outcome**
STD_THREAD mode works. The result differs a little from sequential because the random
numbers come from different generators.


**Input validation in getconfig

Date 11/05/2026
Author Dakota Hernandez
Milestone 2: Sequential and Shared-Memory Baselines
| Type | Design decision / Contribution |

**What happened or what we did**
Made getconfig reject bad input instead of reading whatever was there.

**Result or outcome**
Bad or missing input no longer runs with garbage values.


**Monte Carlo OpenMP implementation

Date 11/06/2026
Author Dakota Hernandez
Milestone 2: Sequential and Shared-Memory Baselines
| Type | Design decision / Contribution / Test (pass) |

**What happened or what we did**
Wrote OPENMPestimate for the Monte Carlo workload and added OpenMP to CMake.

**Details** 

- One parallel region with num_threads(THREADS) and an omp for over the samples.
- Each thread makes its own mt19937 seeded with SEED + thread number.

**Result or outcome**
OPENMP mode works. MPI is still not written.


**Monte Carlo validation tables

Date 11/06/2026
Author Dakota Hernandez
Milestone 2: Sequential and Shared-Memory Baselines
| Type | Test / Timing result |

**What happened or what we did**
Ran the three finished Monte Carlo versions sequential, std::thread, OpenMP over a
range of sizes and thread counts and recorded the results in validation.md in the
Workload 3 folder.

**Details** 

- Sizes: 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16000, 32000,
  64000, 128000, 256000, 500000, 1000000, 2000000, 5000000, 10000000.
- Threads: 1, 2, 4, 8, 16 for std::thread and OpenMP. Sequential only has 1.
- Setup: SEED 12345, REPETITIONS 5, SCHEDULE STATIC, CHUNK 1000, PROCESSES 1.
- Machine: Apple M5, 10 logical cores, macOS, built with clang++ -O2.
- Each row records the RESULT and the average, best, and worst time in ms.


**Result or outcome**
monte carlo basic validation done.

**LOOP std::thread design

Date 11/5/2026
Author Jonah Beck
Milestone 2: Sequential and Shared-Memory Baselines
| Type | Design decision / Pseudocode |

**What happened or what we did**
- Each thread writes its own indices and its own partial[k], so no mutex is needed.
-   STATIC: thread k gets block [k*ceil(n/t), (k+1)*ceil(n/t))
-   CYCLIC: thread k gets chunks k, k+t, k+2t, ...
-   DYNAMIC: threads grab the next chunk from an atomic counter (fetch_add) join all threads, then ad up partial[]

**Result or outcome**
Implemented in threadEstimate().

**LOOP std::thread test

Date 
Author Jonah Beck
Milestone 2: Sequential and Shared-Memory Baselines
| Type | Test (pass or fail) |

**What happened or what we did**
Ran all three schedules with 1, 2, 3, 4, 7 threads (SIZE 1000000, CHUNK 1000).
Expected the same checksum and VALID YES for all.

**Result or outcome**
STATIC  threads=1  checksum=499896499668  avg_ms=0.790242  valid=YES
STATIC  threads=2  checksum=499896499668  avg_ms=0.524785  valid=YES
STATIC  threads=3  checksum=499896499668  avg_ms=0.315976  valid=YES
STATIC  threads=4  checksum=499896499668  avg_ms=0.262825  valid=YES
STATIC  threads=7  checksum=499896499668  avg_ms=0.236824  valid=YES
CYCLIC  threads=1  checksum=499896499668  avg_ms=0.787289  valid=YES
CYCLIC  threads=2  checksum=499896499668  avg_ms=0.430814  valid=YES
CYCLIC  threads=3  checksum=499896499668  avg_ms=0.302382  valid=YES
CYCLIC  threads=4  checksum=499896499668  avg_ms=0.278333  valid=YES
CYCLIC  threads=7  checksum=499896499668  avg_ms=0.525054  valid=YES
DYNAMIC  threads=1  checksum=499896499668  avg_ms=0.878503  valid=YES
DYNAMIC  threads=2  checksum=499896499668  avg_ms=0.542355  valid=YES
DYNAMIC  threads=3  checksum=499896499668  avg_ms=0.392225  valid=YES
DYNAMIC  threads=4  checksum=499896499668  avg_ms=0.293876  valid=YES
DYNAMIC  threads=7  checksum=499896499668  avg_ms=0.304128  valid=YES
