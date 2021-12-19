# A study on performance evaluation of conventional cache replacement algorithms: A review

## Abstract:

Cache Replacement Policies play a significant and contributory role in the context of determining the effectiveness of cache memory cells.
It has also become one of the major key features for efficient memory management from the technological aspect.
Hence, owing to the existing critical computing systems, it has become too essential to attain faster processing of executable instructions under any adverse situations.
In the current scenario, the state of art processors such as Intel multi-core processors for application specific integrated circuits, usually employ various cache replacement policies such as Least Recently Used (LRU) and Pseudo LRU (pLRU), Round Robin, etc.
However, fewer amounts of existing research works are found till date to utter about explicit performance issues associated with the conventional cache replacement algorithms.
Therefore, the proposed study intended to carry out a performance evaluation to explore the design space of conventional cache replacement policies under SPEC CPU2000 benchmark suite.
It initiates and configures the experimental Simple Scalar toolbox prototype on a wide range of cache sizes.
The experimental outcomes obtained from the benchmark suite show that PLRU outperforms the conventional LRU concerning computational complexity and a variety of cache blocks organization.

## Introduction

Though in the recent times various computing technologies have increased the use of high-speed processors and random access memories (RAMs) but there is a need of improvising efficient optimization in data cache memory management hierarchy to bridge the ever increasing speed gap in between CPU and main memory.
Processors are faster in processing operations on operands than the access time of large capacity main memories.
Although various semiconductor memories exist which can be accessed and operated under the speed of a processor for executing instructions, the problems arise regarding cost effectiveness in providing a high-speed main memory embedded with semiconductor memories [1].
The above-stated issue can be mitigated by enabling a small block of cache in between the main memory and processor.
As state of the art processors use multiple levels of caches along with high associative features, it results in low efficiency and robustness during the data access management.
Fig. 1 shows a tentative cache architecture embedded with fully associatively mapping.

![图片](https://user-images.githubusercontent.com/43129850/146665285-5edd4a74-f5d8-47f1-b93d-b5e1a2bc2d5d.png)

Fig. 1.
Fully associative mapping

### A. Effectiveness of Common Cache Replacement Policies

The cache memory map organization is conceptualized in a way which states the situation where cache memory flag value is set to 1 which indicates all the lines in the current cache become full.
Therefore, if any new block of memory is needed to be placed into the cache for processing new instructions, the cache controller must discard a memory line and replace it with the new data obtained from the main memory.
Moreover, it is anticipated that the discarded cache memory line will no longer be used further.
The cache controller decides the cache memory block replacement based on an optimal replacement policy where the memory block, whose next reference points to the furthest away in future in comparison with the other cache blocks presently in the stack, is eliminated.
It is essential to determine the future block references; hence, heuristic methods are applied to eliminate the most eligible cache memory block.
As state of art processors are usually inclined towards employing cache replacement policies such as random, LRU (Least Recently Used), Round Robin or FIFO and PLRU (Pseudo LRU) but very few existing studies are found to point out the best among the above-mentioned replacement strategies [2] [3].
The decision to evaluate the best replacement algorithm is difficult as cache miss happens during the read operation where an instruction is fetched and retrieved from the main memory and placed into the cache.
The higher performance of a cache replacement policy is defined based on its effective cycle time which can be computed regarding cache memory cycle time tcycle and main memory cycle time tmain.
The probability of cache hit is denoted as ph and then the probability of cache miss rate will be (1-ph).
The formula of average cycle time is defined considering only read operation which is highlighted below.

<img src="https://render.githubusercontent.com/render/math?math=\text{T}_{\text{avg}}=\text{t}_{\text{cycle}}+\pm (1-\text{P}_\text{h})\text{t}_\text{main} \qquad (1)">

All the conventional state of art cache replacement policies except the random can detect the cache memory line to be eliminated by looking into its past reference.
In the case of LRU policy implementation, a set of state transition signals (control status bits) is required to update the cache scheduler about when each cache block is accessed.
Therefore, set-associatively in between cache and main memory increases on the number of bits and it imposes cost and computational complexity.
Possibly, the best way to reduce the complexity associated with LRU, the random policy has been chosen but to an extent, most of the researchers and computer designers opted Pseudo LRU heuristic algorithm to minimize the hardware cost and enhance the performance of the system by approximating the LRU mechanism [4]–[5][6].
Though most of the recent studies on cache replacement policies usually incorporate LRU techniques with limited associatively, but few of them initiated the enhancement of LRU by improving replacement decisions.
The proposed study aims to explore the efficient cache replacement policies regarding their cache design space and complexity.
It also defines a performance metrics considering the conventional cache replacement policies and configures a Simple Performance Scalar tool to appraise cache performance under SPEC CPU2000 benchmark set up.
The experimental outcomes also show how pseudo-LRU policies outperform the conventional OPT, LRU, Random, Tree-based and MRU based cache replacement algorithms on wide range of cache, cache size, associatively and cache level.

The rest of the manuscript is organized as follows: Section II discusses the literature survey followed by the theoretical overview of the performance cache estimation discussed in Section III.
Section IV discusses the background of efficient cache replacement policies.
The functionality of garbage collectors on replacement cache operations is discussed in Section V.
Section VI describes the experimental methodology whereas Section VII highlights the experimental results obtained by the evaluation of conventional cache replacement policies.
Section VIII concludes the overall study.

## Literature Review

This section highlights the most recent conventional studies carried out towards optimizing error rates during write operations of SRAM and STT-RAM L2 caches.
The study of authors in [7] incorporated combined cost sensitivity and locality principle to improve the cache performance associated with the cache replacement algorithms.
They have defined a new memory hierarchy which is capable of reducing the average memory access time.
The proposed locality aware cost sensitive cache replacement algorithm maximizes the overall 10 L2 cache performance by up to 85% to 15% while its evaluation on SPEC CPU2000 benchmarks and depicts 44% to 11% on average speed up when evaluated using dual core CPM architecture model.
In [8] a cover set method was introduced to solve the rule dependency issue, and important cache rules occur during the implementation process of OpenFlow switches in Ternary Content Addressable Memory (TCAM).
The performance analysis shows the effectiveness of the proposed method by comparing it with the existing techniques.
In [9] authors have proposed a cache replacement algorithm namely, Least Error Rate (LER) for reducing the write error rates in L2 caches.
The design principles of the proposed algorithm explicitly depict that it acquires minimum error rate in write operation from block incoming requests.
The comparative analysis shows that LER outperforms the conventional LRU algorithm regarding cache write error rate and cache miss rate.
It exhibits the minimization of error rate by 2x with about 1.4% and 3.6% performance overhead as compared to the LRU mechanism.
Authors in [10] developed a very light weight caching management algorithm for maximizing the traffic volume of data instruction.
It also minimizes the bandwidth usage.
The experimental outcomes show that the cache algorithm performs better than the existing strategies regarding access latency, 60% better hit ratio and efficient server bandwidth consumption.
A performance effective regional popularity-aware cache replacement algorithm has been introduced in [11].
It ensures high reliability and reduces unnecessary cache replacements in flash-based solid state drives.
The performance metrics show that it improves throughput by up to 53% and outperforms the conventional cache replacement strategies.
A Random First Flash Enlargement (RFFE) algorithm was introduced in [12] to improve the performance of write operations in L2 caches.
The experimental results show that it outperforms the previously proposed techniques on merging, erase and write count.
The authors in [13] addressed the problem of cache replacements for transcoding proxy caching.
It proposed an efficient cache replacement algorithm to maintain the cache consistency by taking into account different version of same multimedia objects.

## Cache Performance Estimation

### A. Cache Performance

The performance of a cache block is measured regarding hits and miss rates during the instruction execution phase of central processing units (CPUs).
What a cache hits is computed during the access time when cache scheduler finds data in cache and cache miss is the one when data cannot be fetched.
The cache hit computed during the read operation when an entry in the cache is tracked down.
Equation (2) shows the computation of access time on hitting cost, miss rate, and miss penalty.

<img src="https://render.githubusercontent.com/render/math?math=\text{Cache}_ {\text{Access}\ \ \text{Time}} =\eta (\text{hit Cost})+\pm \mu\ (\text{miss rate})*\beta \ (\text{slow memory}\ \text{access time}) \qquad (2)">

The conventional study also put much effort on designing fast control, and it also incorporates optimization algorithms to decrease the cache miss rates.
The cache miss is basically classified into three different types of categories such as conflict misses, capacity misses, and compulsory misses.
The compulsory misses happen during the start up time of a program when data is loaded into the cache for the first time, and it becomes unavoidable.
The term capacity misses indicates that when the data is reloaded into the cache because of its insufficient space for holding up a large scale of data.
All other cache misses are considered as conflict misses where theoretically it can be defined that there is enough space in the cache memory, but the hash function causes cache miss anyway [14].

### B. Fetch and Write Operations

The fetch policy is defined based on the fetch operation in between the main memory and cache blocks.
There are three types of fetch policies namely, a.
Demand Fetch, b. Pre-fetch and c. Selective fetch respectively.

- Demand Fetch Policy

Demand fetch is accessing a cache block on a missing phase.
The existing state of art studies considers it as a very simple mechanism for cache fetching as it doesn't require additional hardware and tags except identifying the cache line i.e.
for replacing.

- Pre-fetch Policy

This kind of fetch strategy incorporates accessing of (j+1)th block in cache memory when the jth block is initially referenced to the exception handling operations.

- Selective Fetch Policy

This cache fetch policy is defined based on some major criterion where main memory is more likely used rather than the caches to hold the data [15].

Figure 2 shows a conventional hierarchy of separate instructions and their associative data caches.

![图片](https://user-images.githubusercontent.com/43129850/146665301-708af6cc-94ec-49f0-80d4-87cc40d23333.png)

Fig. 2.
Conventional data caches and instructions hierarchy

However, write operations can occur in the cache line words, but it is possible that the cache word and the copy of the cache word data which resides in the main memory block can be different.
It is necessary to avoid the alternation of cache word data and copy of that data in main memory in case of shared memory management among multi processor systems.
The cache and the main memory copy should be identical because of multiple input-output transition operations.
Two different types of principles are defined to update the main memory blocks which are write through and write back operations respectively.

- Write Through Policy

The write-through policy is delineated on the time in which the cache scheduler performs a write operation in both main memory and cache.
The average access time of the write through is given in (3) as,

<img src="https://render.githubusercontent.com/render/math?math=\text{T}_\text{avg} \text{(Write}\ \text{through)} =(\text{effective cycle}\ \text{time of}\ \text{cache}\ \text{memory} +\pm \text{fraction of}\ \text{write}\ \text{references)} \qquad (3)">

- Write Back Policy

In this case, the write operation on the main memory can only be performed during the time of block replacement time [16] [17].
The average access time of write back is computed by (4) which is given below:

<img src="https://render.githubusercontent.com/render/math?math=\text{T}_{\text{avg}} (\text{Write back}) =(\text{effective cycle}\ \text{time of}\ \text{cache memory} +\pm (1-\text{P}_{\text{h}})\text{t}_{\text{main}}) \qquad (4)">

## Conventional Cache Replacement Algorithms

This section introduces the conventional cache replacement policies and performs a complexity analysis on complexity of different cache line heuristics.
The cache replacement is required when an on demand word block is not present in the cache during the program or its respective instruction execution phase.
Hence, it is essential to transfer a word block from main memory to the cache and to displace an existing block if the required cache lines/stacks are full.
The conventional cache replacement algorithms perform replacement of existing cache blocks except the case of direct mapping.
The cache replacement policies are usually implemented in hardware utilizing the complete main memory cycle.
It also computes the main memory cycle when a new block is being fetched during program execution [18].

### A. Cache Replacement Policies

There are various existing state of art cache replacement algorithms which need to be discussed to highlight a significant gap in between the OPT policy and LRU policy, as a well-known observation on conventional policies ensures a better future enhancement and optimization of second level data cache performance.
The following are the different types of cache replacement policies:

1. **Random Replacement Algorithm**

Perhaps, a random replacement strategy is considered as one of the simplest algorithms for implementing on a hardware platform.
It selects a word cache block to be replaced in a totally random manner irrespective of considering its memory references or previous selections.


2. **First in First Out Replacement Policy**

This algorithm replaces the block which has been there inside the cache for the longest time.
It is implemented in a FIFO stack considering block addresses of a fully associative cache.
One fully associative cache comprises of a sufficient number of counters and respective bits for identifying the block of words.

3. **LRU Algorithm for Cache Memory**

It usually eliminates the block from the cache which has not been referenced for a long period.
There are several ways of implementing the LRU into the cache which includes: 1. Counters implementation, as well as 2. Register stack implementation.
In register stack implementation, process considers a set of n-bit registers where each block in the set needs to be considered.
The algorithm sets the most recent blocks in the top of the stack and least recently blocks in the bottom of the stack.
The value of each block is internally passed among each other when the blocks are referenced.
This logic poses a very little substantial performance growth in the system and doesn't deal with any practical solutions.
Fig.3 shows the register stack based LRU implementation [20] [21].

![图片](https://user-images.githubusercontent.com/43129850/146665335-19e3d051-636c-42a5-8c8b-c2d1913a2762.png)

Fig. 3.
Register stack method

4. **Pseudo Least Recently Used Policy Based on MRU Bits**

Most of the existing research trends show that though integration of random cache replacement policy into the LRU heuristic reduces cost but potentially it also poses some complexities due to its requirement to track the number of bits when each block is accessed.
Pseudo LRU scheme is found to be the most efficient as it employs a different process to compute the approximation of LRU mechanism for speeding up the operation.
It also reduces the complexity of implementation procedure.
This policy employs nway-1 bits to track down the cache block access during the main memory cycle.
A comparative analysis of different cache line replacement policies is highlighted in Table I [22].

![图片](https://user-images.githubusercontent.com/43129850/146665389-7a22550a-ec77-4317-8c96-541a0cb46160.png)

Table I.
Complexity evaluation of different replacement policies

## Impact of Java's Garbage Collector on Cache Line Replacements

### A. Java's Garbage Collector

It is one of the important features which can be added into Garbage collector which is a mechanism employed by JVM to remove objects which have went out of scope.
In Java, we can create an object by giving a new() keyword but we cannot remove an object unless and until we are using the keyword free() but that is not allowed in Java.
This results in the running of memory leakages.
Garbage collectors take care of releasing objects as in when to go out of scope.

### B. What Garbage Collector Actually Does?

It collects objects that go out of scope.
It uses the Mark and Sweep methodology.
In mark and sweep methodology, it goes across the entire heap and checks which memory addresses are being referenced; then marks those memory addresses which are being referenced, leaving those who are not being referenced as unmarked.
Once this process is done, then it goes back and collects all those memory spaces which have been left unmarked during the process when the application is running.

Let's say the objects are allocated and some of the objects are de-allocated by the garbage collector as they are no longer in use.
Garbage collectors collect object which goes out of scope during any process execution [23].
Thus, it results in fragmentation in the memory space.
In Java, garbage collector uses two different types of procedures to allocate the heap memory using de-fragmentation which is as follows.
1. JVM Shift methods (without indirection)
2. JVM Implementation (with indirection)

![图片](https://user-images.githubusercontent.com/43129850/146665398-29e1c88b-067e-4761-ace6-fabb4ee5fdcb.png)

Fig. 4.
Shift method of JVM for object allocation

The above-mentioned Fig. 4 shows how a garbage collector performs object allocation to the memory spaces using shift method.
It also shows when it makes the shift from the memory address 1000 to 500, thus all the referenced objects from memory address 1000 will be now referencing to the new memory address 500 in a direct way.
Fig. 5 shows that how the indirect method of referencing to allocate objects reduces the processing cost and memory overhead.

![图片](https://user-images.githubusercontent.com/43129850/146665405-ab824e92-9463-4428-80c9-a5aa9b7d5cf1.png)

Fig. 5.
Indirect method of JVM for object allocation

## Experimental Prototyping

The experimental methodology explains that how the performance evaluation set up has been configured considering different types of simulators such as sim-cache and sim-cheetah.
A Simple Scalar tool box is modified for supporting the pseudo-LRU replacement policies and for collecting the statistical estimation of L1D cache misses.
The simulator workload basically considers some of the significant benchmark interval statistics to track down the dynamic behavior associated with each and every cache lines.
The simulator model is initiated considering the filtered out benchmarks from SPEC CPU2000.
SPEC CPU2000 integer and floating point benchmarks are incorporated with reference data inputs.
It considers 500 million instructions and simulates it under a test condition of various cache organizations such as 4, 8, 16 and 32 ways.
The test bed has been simulated for replacement policies like Random, FIFO, LRU, PLRUt and PLRUm.

Initially, the first set of instructions is split and executed under LlI and L1D data caches with 4KB, 8KB, 16KB, and 32KB instruction sizes.
To track down the cache misses during the program and instruction execution, the proposed study initiates the recording of cache misses with 100000 instructions as well as the compact application process.

The cache performance metric has been evaluated using (5) as,

<img src="https://render.githubusercontent.com/render/math?math=\text{Cache}_\text{performance}=1000\times \text{Cache}_\text{missrate}\times\frac{\text{Memory Access}}{\text{Instruction count}} \qquad (5)">

The experimental results of L1D cache misses for different instruction sets (Fig. 6, Fig. 7, Fig. 8, Fig. 9) show that though OPT policy achieves very high efficiency in terms of very less L1D misses as compared to the Pseudo LRU, the latter can be implemented very easily with a short period and irrespective of exploiting the future references of cache blocks repeatedly.

![图片](https://user-images.githubusercontent.com/43129850/146665418-bba1eaeb-5c39-4f91-bdea-aee65ab5fdf6.png)

Fig. 6.
L1D misses per 4K instructions

![图片](https://user-images.githubusercontent.com/43129850/146665423-b8ae8e18-57e2-46fb-949d-f2cd73ee2b88.png)

Fig. 7.
L1D misses per 8k instructions

![图片](https://user-images.githubusercontent.com/43129850/146665426-791ef648-cf6f-420f-85fc-0982866c8f54.png)

Fig. 8.
L1D misses per 18K instructions

![图片](https://user-images.githubusercontent.com/43129850/146665432-c027c7a4-2576-42ff-b6b3-1e9a967cf656.png)

Fig. 9.
L1D misses per 32K instructions

Additional experiment measurements also highlight the cumulative distribution of cache hits and cache misses.
The proposed study concludes pseudo-LRU policy as an efficient one as it depicts very high-efficiency memory management on less computational complexity and execution period.
The proposed study can be referred in future research for exploiting the efficient cache replacement policy.

## Conclusion

As the modern computers demand very high speed, small and efficient processors from the design aspect, hence reducing the memory latency, cache misses have become extremely important to increase a processor's performance.
The proposed study evaluates the performance metrics associated with the conventional cache replacement policies and encompasses its significant impact on future research direction of cache performance enhancement.
The experiment outcomes show that Pseudo LRU policy outperforms the conventional methods on cost effectiveness.
The proposed study considered two different types of Pseudo LRU performance evaluation i.e. Tree based and MRU based.
The purpose of this proposed study is to support the future research of cache power management and prediction considering the exploitation of Pseudo LRU policy.
