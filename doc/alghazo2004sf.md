# SF-LRU cache replacement algorithm

## Abstract:

In this paper we propose a replacement algorithm, SF-LRU (second chance-frequency - least recently used) that combines the LRU (least recently used) and the LFU (least frequently used) using the second chance concept. A comprehensive comparison is made between our algorithm and both LRU and LFU algorithms. Experimental results show that the SF-LRU significantly reduces the number of cache misses compared the other two algorithms. Simulation results show that our algorithm can provide a maximum value of approximately 6.3% improvement in the miss ratio over the LRU algorithm in data cache and approximately 9.3% improvement in miss ratio in instruction cache. This performance improvement is attributed to the fact that our algorithm provides a second chance to the block that may be deleted according to LRU's rules. This is done by comparing the frequency of the block with the block next to it in the set.

## Introduction

One of the most critical design issues in high performance processors is the power consumption. Microprocessor's chip area is dominated mostly by on-chip cache memories and thus the need arises for power efficient cache memories. The main idea behind our research is to improve the efficiency of the first level memory hierarchy by utilizing an efficient novel replacement algorithm (SF-LRU). Therefore, accesses to the larger and more power consuming memory hierarchies are prevented, which reduce the execution time of application and the power consumption. Also a sophisticated high-speed memory hierarchy is essential for computers to bridge the growing speed gap between the main memory and processing elements. In order to improve processor's performance small and fast memories (Cache memories) are utilized to temporarily save the content of segments of main memory that are currently in use. Cache memory is the simplest cost-effective way to achieve high-speed memory hierarchy. It is for this reason that cache memory has been extensively studied since their first introduction. Three common cache organizations have been used since then: fully associative, set associative and direct mapped. Caches from direct mapped to fully associative are in fact increasing levels of set associativity: direct mapped is nothing but one-way set associative and a fully associative cache with n blocks might be called n-way set associativity on the other hand, fully associative can be thought of as having one set and direct mapped as having n sets. Generally, set associative cache organization offers a good balance between hit/miss ratios and the implementation cost. The choice of a block replacement algorithm, in set associative caches, can have a great effect on the overall system performance. [1]–[2][3]

The aptitude of caches to eliminate the performance gap is determined by two main factors: the access time, and the hit / miss ratio. The access time, the time needed to get data from the cache, is critical for the cache memory because a longer access time basically implies a slower processor clock rate. The hit ratio is the number of memory references that can be satisfied by the cache memory. The hit and miss ratio are also critical, both because misses impose delays and the fact that off-chip busses, especially those shared, are limited resource that impose a delay on the processor in waiting for the block to be moved to cache (miss penalty). The locality of reference property used by most programs allow the cache to provide instruction and data required by the CPU at a high rate that is more in alignment with the CPU's demand rate.

Conventional replacement algorithms used are LRU (least recently used), LFU (least Frequently Used), and FIFO (First in First out) Algorithms. Other improved algorithms include LRFU (Least Recently/Frequently Used) algorithm. [1]–[2][3][4][5][6][7][8]

All these algorithms have one common function; which is to reduce the miss rate. The cost of misses includes miss penalty, power consumption, and bandwidth consumption. Relative performance of these replacement algorithms depends primarily on the span of the history reviewed. The LRU algorithm reviews a longer history of past address patterns than FIFO. LFU employs a different type of history than both the LRU and FIFO. The LRU reviews the history to determine which block hasn't been used for the last memory operations to delete it from cache. While the LFU checks the history to determine which memory block hasn't been frequently used to delete from the cache. The LRFU combines both the LRU and LFU algorithms it provides a wide array of replacement algorithms. Researchers are continually proposing techniques to optimize the performance of cache replacement algorithms. [1]–[2][3][4][5][6][7][8]

In order to reduce power consumption we are concentrating on reducing the miss rate. In every miss, the cache controller fetches the requested cache line from main memory to store it in cache. This is the most power consuming operation. In fact, in many systems, it has been shown that the majority of power cost is not due to data-path or controllers but due to the global communication and memory interactions. Embedded applications for signal processing consume 50–80% of the total power in memory traffic alone due to the communication between the processor and the off-chip memory. In fact, it is critical to focus on design strategies that reduce the power consumption due to off-chip memory traffic [10].

## Previous Work

Effective cache replacement algorithms are essential in reducing the number of cache misses. The focus of this paper will be on reducing the power consumption by reducing cache misses through the utilization of novel cost effective replacement algorithm. We therefore start with related work for replacement algorithms then introduce related work for power consumption.

The Most common replacement algorithms used in cache memory are first in first out (FIFO), most recently used (MRU), least recently used (LRU), and least frequently used (LFU). As the name implies, the FIFO algorithm replaces the block in the cache that was referenced first. The MRU algorithm replaces the block in the cache that was referenced the most times. The LRU algorithm replaces the block in the cache that has been unused for the longest time. The LFU algorithm replaces the block in the cache that was least frequently referenced. The LRU and LFU are the two extreme points in which all other variations of replacement algorithms fall. [1]–[2][3][4][5][6][7][8]

Many different methodologies have been proposed by researchers to improve the performance of replacement algorithms. Some of these methodologies are the frequency based replacement (FBR), the least recently frequently used (LRFU), and the segmented least recently used (sLRU). Other proposed algorithms include LRU-K proposed. LRU-K replacement algorithm makes its replacement decision based on the time of the Kth-to-last reference to the block. The FBR algorithm is a hybrid replacement policy that combines both LRU and LFU, maintaining the LRU ordering of all blocks in the cache, but replaces the block in the cache that is least frequently used. The LFRU algorithm associates a value called Recency and Frequency (CRF) to each block in the caches and replaces the block in the cache with the minimum CRF value. [3]–[4][5][6][7][8][9]

There are advantages and drawbacks for each one of the mentioned algorithms. One drawback for the LRU is that it uses only the time of the most recent reference to each block and cannot differentiate between frequently and infrequently referenced blocks. The LFU on the other hand cannot differentiate between references that occurred way in the past and the more recent ones. The LRU-K considers only the Kth reference while ignoring the K-1 references. The LRU-K can differentiate between frequently and infrequently referenced blocks but still does not combine recency and frequency in a unified manner. Other algorithms such as the LRFU, even though combining the recency and frequency in a unified manner has a lot of implementation overhead. These are just some drawbacks of some of the replacement algorithms to show that all replacement algorithms are not perfect and can have room for modification and performance improvement. [1]–[2][3][4][5][6][7][8][9]

Since our main concern is power consumption, we show here some related work to power consumption because many research has been done on reducing the power consumption of cache memories. Researchers have proposed many techniques to reduce power consumption, such as the methods proposed by Memik et al. [11], and the technique proposed by Nicolaescu et al. [12]. The first proposed a victim cache structure to reduce the number of accesses to more power consuming structures, while the latter propose a technique utilizing cache line address locality to determine the cache way prior to the cache access.

## Energy Model

Several energy models have been proposed for caches. We base our energy model basd on the model developed in [13]. Where energy is given by

<img src="https://render.githubusercontent.com/render/math?math=\large {Energy} =hit_{-}rate^{*}Energy_{-}hit+\pm miss_{-}rate^{*}Energy_{-}miss">

where

$\large Energy_{-}hit =Energy_{-}Decoder+\pm Energy_{-}Cell_{-}Arrays$

and

$\large Energy_{-}miss =Energy_{-}hit+Energy_{-}access_{-}memory$

The Energy_Cell_Array is the energy in the cell arrays, Energy_access_memory is the energy required to access data in main memory. And Energy_decoder is the energy in the decoder. As previously stated the Energy required to access data from main memory consumes the majority part of the overall power cost. Thus it is clear that Energy_miss $\gg$ Energy_Hit, And it is also apparent from the previous equations that if miss rate reduction is achieved, then energy consumption is reduced. More information on the complete model can be found in [13].

This proves that our approach to the energy consumption problem is correct and that when we achieve better-hit rate and reduce the number of misses we are actually reducing the power consumption of the system.

## The Sf-Lru Algorithm

In this paper, we introduce a cache replacement algorithm based on LRU and LFU. The algorithm combines both the frequency and recency of blocks in making the decision to replace blocks. The basic philosophy is to modify the LRU so that not only is there a counter to count the number of times a memory block has been referenced but also to give a second chance to those memory reference that have been referenced to the maximum of the counter before being thrown away. This combined number we will call RFCV (Recency-Frequency Control Value).

![图片](https://user-images.githubusercontent.com/43129850/146668577-9c9e2787-4ebb-4dce-976e-d089658aee32.png)

Figure 1:
Hardware representation of the RFCV $\text{RFCV}=Y=(y+1)^{Operation}$

Figure 1 shows hardware architecture of the calculation implementation for the RFCV value. Using this implementation of calculating the RFCV value, the algorithm determines either to delete the last block from the memory or a previous one. The algorithm actually compares the RFCV values for all the blocks in the set and deletes the one with the lowest value:

So, se can conclude that in case of read operation the value of RFCV is increasing value and in case of write it is constant. 

Once we find the block that has to be deleted from the memory, using LRU (F(x)=(1/p)x), we will compare its RFCV value with the previous block in the set. Then, the block that will have to leave the cache will be the one with the smallest RFCV value.

After each comparison operation, in the case that last block in the set has to stay and one of the previous ones has to leave, the last block RFCV has to be recalculated with R=0 which means, it will be set to 1. The idea behind this calculation is that the block had been given one and only one chance to stay in the cache. In order for it stay in the set for a longer period of time, it has to be referenced again so its RFCV value will increase.

![图片](https://user-images.githubusercontent.com/43129850/146668995-bd318093-3a88-486c-b978-468927afe7bf.png)

Figure 3:
SF-LRU operation

Figure 3 shows the flow of SF-LRU algorithm for two cases. For write, the RFCV values are checked for the last two blocks. Since the last is greater than the previous SF-LRU gives a second chance for the last block and evicts the previous. Swapping the two blocks then executing the LRU algorithm accomplishes this task. As shown in the figure that RFCV values are then updated. As for the read operation, if it is a hit, the RFCV values are updated and LRU is executed Figure 2 shows the pseudo code for the SF-LRU algorithm.

![图片](https://user-images.githubusercontent.com/43129850/146668974-be9edc67-585f-4bc5-be7f-a0a8bd141465.png)

Figure 2:
Pseudo-code of SF-LRU algorithm

## Performance Evaluation

We implemented the proposed algorithm using a trace driven simulator. The base configuration is 2–128K bytes, 8 bytes per cache line for I-cache and 32 bytes per cache line for D-cache, 4-way set associative cache. A set of programs is picked up from SPEC92 benchmark suites.

In this section, we present the performance results of the SF-LRU algorithm using benchmarks applied to the simulator. The benchmarks used in this paper are Spice, CC1, and Pasc. The miss rate was measured using the simulator. Figures 4–9 show the simulation results. Through comprehensive comparison between SF-LRU, LRU and LFU, It is shown that the SF-LRU outperforms the latter two. In particular, the SF-LRU can provide a maximum value of approximately 6.3% improvement in the miss ratio over the LRU algorithm in data cache and approximately 9.3% improvement in miss ratio in instruction cache. This improvement in miss rate implies that the number of accesses to lower level memory is reduced thereby reducing the overall power consumption.

SECTION 6.Conclusion
The SF-LRU presented in this paper is a cost effective way of reducing cache miss ratio hence power consumption. The integration of this approach with more advanced techniques on different architectures may further reduce miss ratio. Currently we are exploring all these options
