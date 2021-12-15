# Inter-Reference Gap Distribution Replacement: An Improved Replacement Algorithm for Set-Associative Caches

**Categories and Subject Descriptors**
B.3.2 [Memory Structures]: Design Styles—Cache memories

**General Terms**
Algorithms, Design, Management, Performance

**Keywords**
Cache memory, replacement algorithm, set-associative cache

## ABSTRACT

We propose a novel replacement algorithm, called Inter-
Reference Gap Distribution Replacement (IGDR), for setassociative
secondary caches of processors. IGDR attaches a
weight to each memory-block, and on a replacement request
it selects the memory-block with the smallest weight for
eviction. The time difference between successive references
of a memory-block is called its Inter-Reference Gap (IRG).
IGDR estimates the ideal weight of a memory-block by using
the reciprocal of its IRG. To estimate this reciprocal, it
is assumed that each memory-block has its own probability
distribution of IRGs; from which IGDR calculates the expected
value of the reciprocal of the IRG to use as the weight
of the memory-block. For implementation, IGDR does not
have the probability distribution; instead it records the IRG
distribution statistics at run-time. IGDR classifies memoryblocks
and records statistics for each class. It is shown that
the IRG distributions of memory-blocks correlate their reference
counts, this enables classifying memory-blocks by their
reference counts. IGDR is evaluated through an executiondriven
simulation. For ten of the SPEC CPU2000 programs,
IGDR achieves up to 46.1% (on average 19.8%) miss reduction
and up to 48.9% (on average 12.9%) speedup, over the
LRU algorithm.

1. INTRODUCTION
Recently, the speed-gap between processor and DRAM
has widened [19]. As a cache-miss can cause a stall time of
hundreds of processor cycles, it has become imperative to reduce
cache misses for speeding up computer systems. A high
performance cache replacement algorithm is one method to
reduce cache-misses. Many studies have proposed replacement
algorithms for virtual memory paging, database buffer
caches [11, 6], system buffer caches [8, 3, 9, 20, 5, 10, 13],
and processor caches [12]. A replacement algorithm attaches
a weight to each memory-block; on a replacement request,
it selects the memory-block with the minimum weight. Assume
that the time at which a memory-block will next be
referenced is known; then the optimal algorithm with regard
to miss count is to replace the memory-block that has
the largest time difference between the next reference time
and the current time. This time difference is called forward
distance (FD). The ideal weight of a memory-block
is a monotonically decreasing function of its FD. Practical
replacement algorithms estimate this ideal weight of a
memory-block to use as its weight. The accuracy of the
weight estimation method determines the performance of a
replacement algorithm.
The LRU algorithm [4] attaches a greater weight to a
memory-block that has smaller elapsed time since its last
reference. The LRU algorithm utilizes only a single history
of a memory-block, that is the elapsed time; and estimates
the ideal weight using this history under the assumption
that this history reflects future behavior. Often this assumption
does not hold in practice; therefore, the weight
estimation accuracy of the LRU algorithm is poor [11]. The
LFU algorithm [1, 4] attaches a greater weight to a memoryblock
that has a larger number of references. The accuracy
of this method is poor for memory-blocks whose behavior
varies significantly over time. For example, a poor estimate
is generated for the weight of a memory-block which was
referenced many times in a short period of time initially but
will not be referenced again [13]. This case occurs frequently
in practice; therefore the weight estimation accuracy of the
LFU algorithm is poor.
In this paper, we propose a novel replacement algorithm
based on an estimation method which ismore accurate than
the LRU and LFU algorithms. We call our method Inter-
Reference Gap Distribution Replacement (IGDR). The time
difference between successive references to a memory-block
is called its Inter-Reference Gap (IRG). Figure 1 gives an
overview of the IGDR. IGDR estimates the ideal weight of
