# CMPsim: A Binary Instrumentation Approach to Modeling Memory Behavior of Workloads on CMPs

## ABSTRACT

Chip multiprocessors are the next attractive point in the design space of future high performance processors.
There is a growing need for simulation methodologies to determine the memory system requirements of emerging workloads in a reasonable amount of time.
To explore the design space of a CMP memory hierarchy, this paper presents the use of binary instrumentation as an alternative to execution-driven and trace-driven simulation methodologies.
Using the binary instrumentation tool, Pin, we present CMPsim to characterize cache performance, and data sharing behavior of multi-threaded workloads at the speeds of 4-10 MIPS.

## INTRODUCTION

Recent industry trends show that the future of high performance computing will be defined by the performance of multi-core processors [1, 2, 3].
As a result, processor architects now face key design decisions in designing the memory hierarchy.
Additionally, as parallel applications become common workloads that execute on CMPs, detailed memory characteristics of these emerging workloads are essential in designing an efficient memory hierarchy.
Such characterization and exploratory studies require fast simulation techniques that can compare and contrast the performance of alternative design policies.
This paper demonstrates the use of binary instrumentation tools as an alternative to existing execution-driven and trace-driven methodologies.
Using the binary instrumentation tool Pin, we present CMPsim to characterize application instruction profile and memory system performance of emerging workloads on future processors.

Simulation is a common methodology that is used both for design space exploration and the identification of performance bottlenecks in existing systems.
There exist many free simulators and software tools to investigate the memory system performance of applications.
In general, memory system simulators fall into two main categories: trace-driven or execution-driven [23].
With trace-driven cache simulators, address traces of an application are read from an address trace file and fed to a cache simulator (e.g. Dinero IV [11]).
Such simulators rely on existing tools to collect an applications memory address trace and log them to file for later use.
Execution-driven cache simulators rely on functional/performance models to execute an application binary.
The memory addresses generated by the functional/performance model are fed, in real time, to a cache simulator modeled within the functional/performance model.
Since functional models of modern ISAs are slow and can be complex to build, trace-driven simulation is a popular technique for conducting memory performance studies [23].

The usefulness of trace-driven simulation, however, lies in the continued availability of memory address traces to study the memory performance of different workloads.
With several emerging application domains, understanding the memory behavior and working set requirements of different applications requires the ability to generate address traces by just about anyone.
Address trace generation for a target ISA can require sophisticated hardware tools [23] or functional models that not only support the target ISA but also the requirements of the workload (e.g.
the functional model must provide support for multiple contexts if executing a multi-threaded workload).
Such infrastructure to capture memory address traces can be expensive and/or complex to build.

Even if address trace generation were trivial, a practical problem with storing memory address traces is that the address traces can be large, potentially occupying several gigabytes of disk space even in their compressed formats.
Consequently, transferring and sharing large address traces between different locations can be inconvenient.
Furthermore, another problem of using address traces is that the trace is only representative of the compiler and compiler optimizations used to compile the workload.
As a result, studying the behavior of different compiler optimizations of the workload requires the creation of address traces for each compiler and compiler optimization type.
Ideally, a desirable approach for conducting memory performance studies is to have the benefits of the execution-driven methodology without incurring the associated slow speeds and complexities.

To address the drawbacks of current methodologies, the main contribution of this paper is to illustrate the use of existing binary instrumentation tools to conduct quick memory performance studies.
We introduce, CMPsim, a memory system simulator based on the binary instrumentation tool called Pin [4, 16].
With Pin serving as the functional model that provides CMPsim with memory addresses, CMPsim can process memory requests generated by a workload in real time.
Thus, CMPsim simulates the memory system performance of a workload without the overhead of dealing with large address trace files.
Furthermore, CMPsim can simulate the memory system performance of a multi-threaded workload on a CMP with single or multi-threaded cores.
For example, when simulating a four-threaded workload, CMPsim can model a four-core CMP with one thread per core, a two-core CMP with two threads per core, or a single-core processor with four threads per core.

CMPsim is fully configurable and can gather detailed application statistics on cache performance as well as the total amount of data shared between different threads of an application.
Specifically, a user can set the cache parameters, allocation/replacement policies, and write policies.
A user can also specify the number of levels in the cache hierarchy with specifications on the type of inclusion policy.
Even more, the user can configure some levels of the cache hierarchy to either be shared or private amongst different threads/cores of the simulated CMP.
CMPsim also provides application instruction profiles, detailed cache statistics on cache reuse, number of cache hits and misses, number of conflict misses, and number of writebacks.
CMPsim can gather these statistics for billions of instructions of an application (or regions of application specified by the user) at the rate of 4-10 million instructions per second (MIPS).

Unlike existing simulation techniques, the main advantages of CMPsim are: it is a parallel memory system simulator that can process memory requests from multiple threads at the same time; it is fast; it is flexible???users can model any kind of cache hierarchy; it can model multi-cores and multi-threaded cores; it can easily run complex applications like Oracle and Java without any user support; and finally it is relatively simple when compared to full performance models, thus, making it easy to extend or modify.

We used CMPsim to characterize the memory behavior of the multi-threaded workload ammp from the SPECOMP benchmark suite [5].
We present the per-thread instruction profile and the time varying cache and sharing behavior of ammp when run to completion.
The detailed characterization of ammp reveals that it exhibits mostly read data sharing with 30% of its shared data referenced 50-80% of the time.
Additionally, since most of the memory references are to the shared data, ammp benefits greatly from a shared last-level cache configuration compared to a private last-level cache configuration.

## BACKGROUND

### PIN ??? A BINARY INSTRUMENTATION TOOL

Pin is a dynamic binary instrumentation tool that instruments an application that executes on Intel processors.
Pin supports the instrumentation of Linux, MacOS, and Windows binary executables (without the need for source code) for Intel?? Xscale??, IA-32 (x86 32- bit), IA-32E (x86 64-bit), and Itanium?? processors.
Pin is similar to the ATOM toolkit for Compaq's Tru64 Unix on Alpha processors [22].
Like ATOM, Pin provides an infrastructure for writing program analysis tools called Pin tools.

The two main components of a Pin tool are: instrumentation and analysis routines.
Instrumentation routines utilize the rich API provided by Pin to insert calls to user defined analysis routines.
These calls are inserted by the user at arbitrary points in the application instruction stream.
Instrumentation routines define the characteristics of an application to instrument.
Analysis routines are called by the instrumentation routines at application run time.
For example, using the Pin API [4], a user can write an instrumentation routine to instrument every instruction executed by an application.
If the instrumentation routine sets up a call to a user defined analysis routine DoCount() (which simply increments a counter), then the Pin tool counts the total number of dynamic instructions executed by the program.
Besides writing such simple utilities, Pin provides many other advanced features to conduct a variety of microarchitecture studies.
For example, customized Pin tools can profile the static or dynamic distribution of instructions executed by a given application, determine the outcomes of branch instructions and their associated branch targets, acquire effective addresses of all memory instructions executed, change architectural state of registers [20].
With such information, users can write customized Pin tools that model branch predictors, cache simulators, and simple performance models.

Besides instrumenting single-threaded applications, Pin also supports the instrumentation of multi-threaded applications.
The scheduling of different threads of the application is controlled by the operating system.
To distinguish between the different threads of the application, Pin assigns each thread with a unique ID which is different from the native process ID assigned by the operating system.
Pin assigns the first thread, i.e. the main thread, with thread ID 0 and each additional new thread is assigned the next sequential ID, i.e. 1, 2, 3, and so on.
Thus, when conducting studies with a four-threaded workload, Pin distinguishes between threads by assigning the main thread with thread ID 0, and the three remaining threads with thread IDs 1, 2 and 3.
It is the responsibility of the Pin tool to distinguish instrumentation based on different thread IDs.

### RELATED WORK

Several studies have used trace-driven or execution-driven methodologies to characterize the memory behavior and performance of different types of applications.
Uhlig et al. [23] provide a detailed survey of existing trace-driven methodologies.
Iyer et al. [13] introduced a trace-driven simulation framework called CASPER to explore different cache organization alternatives, prefetching mechanisms, coherence protocols and other research studies.
Jaleel et al. [14] used Pin to conduct a detailed memory characterization study of parallel data-mining bioinformatics workloads on CMPs.
Nurvitadhi et al. [18] used an FPGA based cache model (PHASE) that connects directly to the front-side bus to understand the L3 cache behavior of SPECjAppServer and TPC-C. Abandah et al. [6, 7] proposed a configuration independent approach to analyze the working set, concurrency, communication patterns, as well as sharing behavior of shared memory applications.
They present a tracing tool called Shared-Memory Application Instrumentation Tool (SMAIT) to measure different sharing characteristics of the NAS shared-memory applications.
Barroso et al. [9] characterized the memory system behavior of commercial workloads such as Oracle, TPC-B, TPC-D, and AltaVista search engine.
They did their characterization of the memory system behavior using ATOM [22], performance counters on an Alpha 21164 as well as the SimOS simulation environment.
Woo et al. [24] characterized several aspects of the SPLASH-2 benchmark suite.
They used an execution-driven simulation with the Tango Lite tracing tool [12].
Perl et al. [19] studied Windows NT applications on Alpha PCs and characterized application bandwidth requirements, memory access patterns, and application sensitivity to cache size.
Chodneker et al. [10] analyzed the time distribution and locality of communication events in some message-passing and shared-memory applications.

The work presented in this paper differs from prior work in that it presents binary instrumentation as an alternative approach to study cache performance of workloads.
We introduce CMPsim, a CMP cache simulator that can characterize the memory behavior of single and multi-threaded workloads.
We believe that CMPsim also fills the gap on the lack of simple x86 performance tools to characterize the  memory behavior of applications across different memory system configurations.
Full system simulators such as Bochs [15] and Simics [17] support the x86 ISA, however they emulate an entire system with peripherals and an operating system.
Even though such tools are valuable for research, characterizing the memory behavior of individual workloads on such systems can be non-trivial and rather slow.
Unlike existing simulators, CMPsim can characterize the behavior of applications over their entire run or periods of interest defined by the user without using tracing mechanisms, performance counters, or bus sniffers.
Existing work has demonstrated the usefulness of Pin to conduct performance analysis of different applications.
Reddi et al. [20] discuss the use of Pin as a tool for computer architecture research and education.
We demonstrate a working example of CMPsim based on Pin that can be used to characterize application behavior and memory performance of emerging x86 workloads on uni-processors or CMPs.

## CMPSIM ??? AN INSTRUMENTATION BASED SIMULATOR

The interfaces to most binary instrumentation tools are API calls that allow users to hook in their instrumentation routines.
In Pin, the API call to INS_AddInstrumentationFunction() allows for a user to instrument programs based on a single instruction while the TRACE_AddInstrumentationFunction() allows the user to instrument programs at a trace (multiple basic blocks) level.
CMPsim uses these two API routines to set up calls to instrumentation routines Instruction() and Trace().
These two instrumentation routines in turn call the two main analysis routines MemReference() and IProfile() which are responsible for cache simulation and instruction profiling respectively.
We now provide a detailed description of the instrumentation and analysis routines responsible for cache simulation and instruction profiling.
Figure 1 illustrates an implementation overview of the CMPsim simulator.

### CACHE SIMULATION

To simulate the cache behavior of a workload, all memory references generated by a workload must be captured and played through a cache model.
In CMPsim, the Instruction() instrumentation routine sets up the call to the MemReference() analysis routine every time an instruction that references memory is executed.
The primary function of the MemReference() analysis routine is to simulate the cache behavior of the application based on user defined cache parameters.
MemReference() takes as input the thread ID of the instruction, the type of memory operation (read/write), the memory address, and finally the size of the memory operation (in bytes).
This analysis routine interfaces with a cache model that handles user defined cache sizes, associativity, and allocation and replacement policies.
The user has the option of specifying the number of levels in the cache hierarchy.
The user can also specify the caches in the hierarchy to be write- through or write-back and the levels of the hierarchy to be inclusive or non-inclusive.
Furthermore, the cache model can simulate some levels in the cache hierarchy to be private or shared amongst different threads/cores of a CMP.
This functionality allows for a user to simulate a CMP with cores that are single-threaded or multi-threaded.
Finally, if multi-threaded workloads are simulated with CMPsim, an invalidate-based cache coherence protocol (similar to MESI) is also implemented.
On a write request, invalidates are sent to all relevant private caches to invalidate any matching tag entries.
Similarly, on read requests missing in the private cache, remote dirty lines (if any) are required to perform a write-back before servicing the miss in the private cache.

When writing parallel programs, common software programming practice requires that data structures that are shared in a read/write fashion by more than one software context be guarded by a lock to ensure correct behavior.
For example, if CMPsim modeled a shared cache without the use of locks, incorrect behavior can occur if two threads simultaneously miss in the cache with different memory addresses that map to the same set in the cache.
If the shared cache was not guarded by a lock, both threads can read the same LRU state and fill into the same LRU entry (defeating the purpose of the LRU).
Such problems can be avoided by guarding accesses to shared caches by the use of locks.
However, a lock per cache can slow the speed of simulation especially if multiple threads contend for the same cache.
This source of performance bottleneck was avoided by using a lock per cache set rather than a lock per cache.
Thus, during simulation, simultaneous accesses to different sets of the same cache can occur concurrently while simultaneous accesses to the same set of a cache are serialized.

### INSTRUCTION PROFILING

An important aspect of workload characterization is the dynamic distribution of the instruction/opcode mix in an application.
This provides insight on whether an application is dependent on memory behavior, compute behavior, or control flow behavior.
To determine the  instruction profile of an application, CMPsim uses the Trace() instrumentation routine to track all basic blocks executed by a program.
Instrumenting an application on a basic block basis rather than calculating the instruction profile on a per-instruction basis reduces the total instrumentation overhead.
For each basic block, the Trace() routine classifies instructions within the basic block into several different categories based on the specifics of Pin???s internal x86 instruction decoder XED.
For example, instructions are categorized into control instructions, memory read instructions, memory write instructions, integer instructions, and floating point instructions.
The Trace() routine then sets up a call to the IProfile() analysis routine.
The arguments passed are: thread ID, pointer to an internal data structure that holds profile information relevant to the basic block, and the instruction count of the basic block.
Upon receipt of the different inputs, the IProfile() analysis routine increments per thread counters to track the total number of instructions executed for each thread.

In addition to doing instruction profiling, the Trace() instrumentation routine is also responsible for setting up calls to the MemRef() routine to conduct instruction cache simulation.
The Trace() routine passes to the MemRef() routine the thread ID, the PC of the first instruction in the basic block, and the size of the basic block in bytes.
Similar to instruction profiling, instruction cache simulation is done on a basic block basis (rather than per-instruction basis) to reduce the instrumentation overhead.

### CHARACTERIZING USER DEFINED REGIONS OF SIMULATION

Besides characterizing the memory behavior and instruction profile of an application over its entire run, CMPsim can also instrument user defined regions of code.
If the source code of the application is available to the user, CMPsim provides an API (in the form of a header file) to the user to define start and stop regions into the source code.
Specifically, a user can insert a function call to PINStartInstrumentation() at the start of the region(s) of interest and a call to PINEndInstrumentation() at the end of the region(s) of interest.
With this feature, CMPsim dynamically detects user defined start and stop regions and will only instrument applications during the user defined regions of code.
However, if source code is unavailable, Pin???s API provides mechanisms to skip user-defined instructions or instrument only user-defined functions.

### GATHERING STATISTICS WITH CMPSIM

CMPsim can gather a variety of statistics for an application.
CMPsim tracks the application instruction profile, the total number of cache accesses and misses, the sharing characteristics of multi-threaded applications, and coherence traffic.
These statistics are output to a data file when the program finishes execution.
However, to characterize the time varying behavior of the application, these statistics can also be logged periodically to the output file.
The interval between printing statistics to file is defined by the user, hence allowing a user to visualize the time varying behavior of an application over its full run.
This is useful in identifying representative regions of execution for detailed simulation.
In general, statistics are not reset at output time (unless specified), instead the statistics are logged to file and are processed later by a script to determine the application behavior between different logs to the statistics file.
We describe in detail the important statistics that can be measured using CMPsim.
- ApplicationInstructionProfile:Programinstructionsareclassifiedintomemoryandnon-memoryinstructionsthatbelongtodifferent categories of the x86 ISA. Examples of some categories are: branch, jump, ALU, floating-point, semaphore, push, pop, and SSE type instructions.

On a per-cache basis, CMPsim measures a variety of cache statistics, the most important ones being:
- Cache Accesses/Misses/Writebacks: Cache accesses are categorized into hits and misses based on the cache access type. We present cache statistics in terms of accesses/1000 instructions, misses/1000 instructions and miss-rate. For writeback caches, the model also tracks the total number of outstanding writebacks and writebacks to subsequent levels of memory.
- DataReuse:Ausefulmetricbesideshit/missinformationisameasureonthenumberoftimesablockisreusedafteritisallocatedin the cache. CMPsim tracks the number of times a block is referenced (on a demand basis) after allocation in the cache. A demand reference is essentially a request initiated by the processor. For example, a load or store access to the cache is a demand request, where as a writeback and remote snoop access is not a demand request. When a block is evicted from the cache, a histogram is maintained using the total number of references to the block before it is evicted. This metric indicates the average number of times a block is reused during its lifetime in the cache.

CMPsim uses the following metrics to measure the degree of data sharing within the application:
- SharedCacheLine:Forparallelapplications,itisusefultomeasuretheamountofdatasharedbetweenthreads.Wedefineashared cache line as a cache line that is accessed by more than one core during its lifetime in the cache.
- Shared Access: An access to a cache line in shared state is defined as a shared access. This metric is a measure of the variation and frequency of accesses to cache lines in shared or private state. It indicates the amount of data sharing in an application which is useful in deciding whether the application would benefit from a shared cache or a private cache.
- Active-Shared Access: An active shared access is an access to a shared cache line with the condition that the last core that accessed the shared cache line is different from the current core. For example, if the accesses to a shared cache line are represented by the following core ids: ...1, 2, 2, 2, 1, 3, 4, 3, 2, 2, 2, 3, 2..., the accesses by the underlined core IDs are active shared accesses. This metric identifies whether a workload shares cache lines interactively or in a serial fashion. This is useful in deciding whether or not to move cache lines to banks closer to the accessing cores.
- Coherence Invalidate Frequency: The coherence invalidate frequency is measured as the total number of invalidate hits to remote cache lines. This metric is measured on a per 1000 instructions basis and is useful for measuring the amount of read-write sharing in an application.
- Coherence Writeback Frequency: The coherence writeback frequency is measured as the total number of hits to dirty data in remote private caches. This metric is measured on a per 1000 instructions basis and is useful for measuring the amount of read-write sharing in an application.

## CHARACTERIZATION OF WORKLOADS USING CMPSIM

### EXPERIMENTAL METHODOLOGY

Figure 2 illustrates the use of CMPsim to model a four-core CMP with one thread per-core.
We model a three level cache hierarchy.
The L1 and L2 caches are private to each core and the L3 cache is either configured to be private or shared.
We assume a zero latency crossbar interconnection network between L2 and L3.

We used CMPsim to characterize the run time memory behavior of the ammp workload from the SPECOMP suite with the reference input set [5].
We only capture data cache traffic to measure data sharing between different threads of the workload.
The L1 data cache is 32KB, 2-way set associative, with 64B line size and write-through policy.
The L2 cache is 256KB, 4-way set associative, with 64B line size and write-back policy.
Finally, the L3 cache is 2MB, 8-way set associative, with 64B line size and write-back policy.
All caches allocate on a store miss and use the LRU replacement policy.
Additionally, inclusion is enforced between all levels of caches and an invalidate-based MESI cache coherence protocol is modeled.

The SPECOMP workload ammp is run with 4 threads on a dual processor system of Intel?? Pentium?? 4 3.2 GHz processors with hyper- threading enabled.
The workload is compiled using the icc compiler, with optimization flags -O3.
We ran the workload to completion and logged statistics to file every 10 million instructions executed (on a per thread basis).
After simulation, the behavior of the workload over the different intervals of execution is depicted graphically using a post-processing perl script.

### RESULTS

The total simulation time to run ammp to completion was approximately 28 hours.
This implies that CMPsim can characterize workloads at the speed of ~5 million instructions per second (MIPS).
The speed of CMPsim is a function of the memory instruction profile of the workload???the larger the proportion of memory instructions in the workload, the slower the speed of simulation.
For a variety of workloads (not described in this paper) we have observed simulation speeds of 4-10 MIPS.

#### INSTRUCTION PROFILE

Table 1 shows the dynamic instruction profile for the ammp workload when run to completion.
The instruction profile reveals that the workload is floating-point intensive with 67% of total instructions composed of x87/floating-point ALU and ALU memory read operations.
Furthermore, the workload is memory read-intensive with 30% memory read instructions and 13% memory write instructions.
Additionally, the workload has a large basic block length of 10 instructions.
This information confirms with existing knowledge of the workload being a floating-point intensive scientific workload.

#### CACHE PERFORMANCE

Figure 3a shows the number of L1 cache accesses and misses of ammp when run to completion.
The x-axis represents the total number of instructions (in billion) and the y-axis represents the accesses per 1000 instructions and miss rate of the L1 cache.
To capture the time varying behavior of the workload, we present the phase behavior of the workload at a ten million instruction granularity (represented by the solid lines).
The cumulative behavior of the workload is represented by the dashed line.
The figure shows that the workload has a periodic pattern with approximately 90 loops.
Since the workload exhibits a periodic behavior, we show the cache behavior only for the region between 180 and 250 billion instructions.
This region was chosen arbitrarily and the behavior of each loop in this region is representative of all the loops.

Figures 3b, 3c, 3d, and 3e illustrate the L1, L2, private L3, and shared L3 cache behavior for the selected region of execution.
For each cache, we present the total number of cache accesses per 1000 instructions, the total number of cache misses per 1000 instructions, and the miss-rate.
The figure shows a periodic behavior in the cache access and miss pattern with one period spanning 4 billion instructions.
Each loop begins with a cache miss rate as high as 95%.
The cache miss rate reduces to 40% during the second half of the loop because of improved locality.
Despite the existence of locality, the large miss rates in the L2 cache imply a working set larger than the size of the L2 cache (256KB).

Figures 3d and 3e shows the L3 cache behavior of ammp for  private and shared configuration.
The size of the L3 cache in the shared configuration is 2MB and the size of each L3 cache in the private configuration is 512KB.
On average, the shared configuration has 25% fewer misses than a private configuration.
This is because the larger effective capacity of a shared cache allows it to accommodate to the variable working set of each core.
Additionally, if multiple cores share a cache line, a shared cache avoids duplication by having a centralized copy of the line, which increases the effective cache capacity and reduces miss rate.
In a later section, we show that ammp benefits from a shared cache primarily because of extensive data sharing.

#### CACHE REUSE

Figures 4a, 4b, 4c, and 4d show the reuse distribution of lines evicted from the L1, L2, and L3 cache.
Recall that reuse is measured as the total number of times a cache line is greferenced by a demand access after it is installed.
Let reuse be defined by the symbol: <img src="https://render.githubusercontent.com/render/math?math=\rho">.
Reuse values are classified as <img src="https://render.githubusercontent.com/render/math?math=\rho = 0, 1, 2, 3, 4, 5, 6, \text{ and } 7">, and regions <img src="https://render.githubusercontent.com/render/math?math=8 \leq \rho <16, 16\leq \rho <32, \dots 4096\leq \rho <8192, \text{ and } \rho \geq 8192">.
The figures show reuse value of zero at the bottom of the graph and a reuse value of 8192 or higher at the top of the graph (We recommend viewing a soft copy or a color print out of this graph).
On average, the figure shows that 76%, 12%, 10%, and 30% of lines evicted from the L1, L2, private L3, and shared L3 respectively are reused at least once.
The low value of reuse of lines in the L2 and L3 cache is due to a high miss rate causing lines to
be evicted before they get referenced again.
In addition, the poor reuse of data in the shared L3 cache is because temporal locality of data is short lived.
The low value of reuse indicates an inefficient use of the cache.
Since 5???95% of the lines evicted from the different levels of cache hierarchy are never referenced after installation, implying that cache performance can be improved by not installing these lines.
This motivates investigating alternative cache allocation policies to reduce cache pollution and improve effective cache capacity.

#### DATA SHARING

Figure 5a presents the distribution of cache lines shared between different cores of the CMP.
The x-axis represents the total number of instructions and the y-axis represents the distribution of cache lines that are either private, or shared between two, three, or four cores.
The bottom-most segment represents private cache lines, followed by cache lines shared by two cores, three and four cores respectively.
Figure 5a shows that half the cache is shared by two or more cores.
Figure 5b provides more insight into the frequency of access to shared data by presenting the distribution of accesses to the shared last-level cache.
The figure shows as much as 50-80% of the cache accesses are to lines that are shared by two or more cores.
Furthermore, Figure 5c also shows that 50-80% of accesses are to cache lines that are actively shared.
Thus, ammp exhibits a significant amount of data sharing between multiple cores because of which it significantly benefits from a shared cache compared to a private cache.

#### COHERENCE TRAFFIC

Figure 5d presents the coherence invalidate frequency and the coherence writeback frequency for the L1 and L2 cache.
The figure shows that cache lines are invalidated at a rate of 0.1 to 1.5 and 0.1 to 3 invalidates per 1000 instructions from the L1 and L2 cache respectively.
The number of invalidates are higher in the L2 cache because the size of the L2 is larger than the L1.
a given interval, a large number of snoop hits and invalidate hits indicates a large amount of read/write data sharing.
The figure shows that the beginning of each loop exhibits read/write sharing due to the existence of snoop hits (0.1 to 2 hits per 1000 instructions) and invalidate hits (0.1 to 3 invalidates per 1000 instructions).
The interconnection network is stressed heavily at the beginning of each loop due to the large amount of read/write sharing.
On the other hand, the interconnection network is not stressed during the rest of the loop due to very little read/write sharing.
Thus, even though the average bandwidth demand is low, a high bandwidth interconnection network is required to handle the clustered coherence traffic.

## CONCLUSIONS

This paper proposes the use of binary instrumentation to conduct memory performance studies as an alternative to execution-driven and trace-driven methodologies.
Using the binary instrumentation tool Pin, we present a memory system simulator, CMPsim, that is fast, flexible, easy to use, and simple to modify.
We demonstrated the use of CMPsim, to measure the dynamic instruction profile and memory system performance of a multi-threaded workload, ammp, when run to completion.
Our experience shows that binary instrumentation based simulation allows for full characterization of workloads run to completion at speeds ranging from 4-10 MIPS.

## ON-GOING WORK

### SPECULATION

The work presented in this paper assumes no speculative execution, i.e.
perfect branch prediction.
However, with binary instrumentation based simulation (using Pin) it is also possible to model wrong-path execution.
This is accomplished by modeling a branch predictor and  executing instructions down a path predicted by the branch predictor.
To correctly model the effects of wrong-path execution using binary instrumentation, the binary instrumentation tool must allow a user to create check points and change architectural state.
This is required if the branch predictor makes an incorrect decision resulting in wrong-path execution.
To resolve wrong-path execution, after some delay (as specified by the user), the checkpoint must be restored to allow correct program execution.
The addition of speculative execution in our CMPsim infrastructure is part of our on-going work.
We would like to mention here though that the miss rates on a Pentium?? III (using the PAPI performance counter suite) and a simulation of the P3 memory hierarchy (using CMPsim) revealed that the cache miss rates with CMPsim were within 5-10% of performance counter values for various single-threaded workloads.

### TIMING MODELS

The use of binary instrumentation can also be used to perform processor performance/sensitivity studies.
For example, one can model an in-order processor using the CMPsim simulator.
By assigning a latency of execution Tc for an instruction belonging to instruction category c, penalty Tb for a branch misprediction, access latency Tl for level l in the cache hierarchy, and access latency of Tm to memory, one can estimate the performance of a workload using the following equation:

<img src="https://render.githubusercontent.com/render/math?math=\Large \alpha_cT_c+\pm \beta T_B +\pm \sum A_l T_l +\pm \eta T_m">

Where <img src="https://render.githubusercontent.com/render/math?math=\alpha"> is the number of instructions executed, <img src="https://render.githubusercontent.com/render/math?math=\beta"> is the number of branch mispredictions, A is the number of accesses, and <img src="https://render.githubusercontent.com/render/math?math=\eta"> is the
number of misses to memory.
Note that this simple in-order timing model assumes stall on miss and not stall on use.

Using the above simple timing model, and assuming single cycle execution for all instructions, perfect branch prediction (?? = 0) and latencies of 3, 10, and 35 to the 1st level, 2nd level, and 3rd level cache, and finally 100 cycles of latency to memory, we present in Figure 6 the instructions per cycle and the L1, L2, and L3 cache miss rates of gcc and mcf from the SPEC2000 benchmark suite.

From Figure 6a, we observe that gcc has several phases of execution which exercise the different levels of the cache hierarchy.
As expected, the figure shows that gcc performs best when it fits well in the L1 cache.
When it does not fit in the L1 cache, we observe phases of execution (~1.1 and ~11.5 billion instructions) where the performance of gcc is dependant on fitting in the L2 cache.
In general, based on the figure, we conclude that the performance of gcc is sensitive to L2 cache size.
Similarly, from Figure 6b, we observe that mcf exhibits loop behavior in the last 40 billion instructions executed.
The figure shows phases of execution where the working set of mcf is larger than L2 hence we see periods of execution where the miss rate of L2 is a 100%.
In general, we observe separate phases of execution where the performance of mcf is sometimes dependent on the miss rate of the L2 cache and sometimes on the miss rate of the L3 cache.

The above rudimentary timing model can be extended to incorporate dependency between instructions and analytically model stall-on- use in-order processors.
Furthermore, the CMPsim infrastructure can also be used to analytically model an out-of-order processor as proposed by Karkhanis et al [25].
These analytical models work well on first-order with single-threaded workloads or one thread of a multi- threaded workload.
However, estimating the performance of a multi-threaded workload is non-trivial as there is no notion of time (to measure inter-thread contention) with binary instrumentation based simulation.
This is part of our on-going work.

### OTHER MEMORY HIERARCHY STUDIES

Besides cache performance studies, CMPsim can also be used to explore the design space of TLBs and data prefetching.
We are also using CMPsim to investigate novel cache replacement policies to improve the reuse of lines in the cache.
Finally, we are also extending CMPsim to study quality of service studies when executing multi-programmed workloads on CMPs.
