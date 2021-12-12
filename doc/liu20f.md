# An Imitation Learning Approach for Cache Replacement

https://proceedings.mlr.press/v119/liu20f.html

## Abstract

Program execution speed critically depends on increasing cache hits, as cache hits are orders of magnitude faster than misses. To increase cache hits, we focus on the problem of cache replacement: choosing which cache line to evict upon inserting a new line. This is challenging because it requires planning far ahead and currently there is no known practical solution. As a result, current replacement policies typically resort to heuristics designed for specific common access patterns, which fail on more diverse and complex access patterns. In contrast, we propose an imitation learning approach to automatically learn cache access patterns by leveraging Belady’s, an oracle policy that computes the optimal eviction decision given the future cache accesses. While directly applying Belady’s is infeasible since the future is unknown, we train a policy conditioned only on past accesses that accurately approximates Belady’s even on diverse and complex access patterns, and call this approach Parrot. When evaluated on 13 of the most memory-intensive SPEC applications, Parrot increases cache miss rates by 20% over the current state of the art. In addition, on a large-scale web search benchmark, Parrot increases cache hit rates by 61% over a conventional LRU policy. We release a Gym environment to facilitate research in this area, as data is plentiful, and further advancements can have significant real-world impact.

1. Introduction
Caching is a universal concept in computer systems that
bridges the performance gap between different levels of data
storage hierarchies, found everywhere from databases to operating systems to CPUs (Jouppi, 1990; Harty & Cheriton, 1992; Xu et al., 2013; Cidon et al., 2016). Correctly selecting what data is stored in caches is critical for latency, as
accessing the data directly from the cache (a cache hit) is
orders of magnitude faster than retrieving the data from a
lower level in the storage hierarchy (a cache miss). For example, Cidon et al. (2016) show that improving cache hit
rates of web-scale applications by just 1% can decrease total latency by as much as 35%.
Thus, general techniques for increasing cache hit rates
would significantly improve performance at all levels of the
software stack. Broadly, two main avenues for increasing
cache hit rates exist: (i) avoiding future cache misses by
proactively prefetching the appropriate data into the cache
beforehand; and (ii) strategically selecting which data to
evict from the cache when making space for new data (cache
replacement). Simply increasing cache sizes is a tempting
third avenue, but is generally prohibitively expensive.
This work focuses on single-level cache replacement (Figure 1). When a new block of data (referred to as a line) is
added to the cache (i.e., due to a cache miss), an existing
cache line must be evicted from the cache to make space for
the new line. To do this, during cache misses, a cache replacement policy takes as inputs the currently accessed line
and the lines in the cache and outputs which of the cache
lines to evict.
Prior work frequently relies on manually-engineered heuristics to capture the most common cache access patterns, such
as evicting the most recently used (MRU) or least recently
used (LRU) cache lines, or trying to identify the cache lines that are cache-friendly vs. cache-averse (Qureshi et al.,
2007; Jaleel et al., 2010; Jain & Lin, 2016; Shi et al., 2019).
These heuristics perform well on the specific simple access
patterns they target, but they only target a small fraction
of all possible access patterns, and consequently they perform poorly on programs with more diverse and complex
access patterns. Current cache replacement policies resort
to heuristics as practical theoretical foundations have not
yet been developed (Beckmann & Sanchez, 2017).
We propose a new approach for learning cache replacement policies by leveraging Belady’s optimal policy (Belady, 1966) in the framework of imitation learning (IL), and
name this approach PARROT.(Parrots are known for their ability to imitate others.) Belady’s optimal policy (Belady’s for short) is an oracle policy that computes the theoretically optimal cache eviction decision based on knowledge of future cache accesses, which we propose to approximate with a policy that only conditions on the past accesses.
While our main goal is to establish (imitation) learned replacement policies as a proof-of-concept, we note that deploying such learned policies requires solving practical challenges, e.g., model latency may overshadow gains due to
better cache replacement. We address some of these challenges in Section 4.5 and highlight promising future directions in Section 7.
Hawkeye (Jain & Lin, 2016) and Glider (Shi et al., 2019)
were the first to propose learning from Belady’s. They train
a binary classifier to predict if a cache line will soon be
reused (cache-friendly) or not (cache-averse), evicting the
cache-averse lines before the cache-friendly ones and relying on a traditional heuristic to determine which lines
are evicted first within the cache-friendly and cache-averse
groups. Training such a binary classifier avoids the challenges (e.g., compounding errors) of directly learning a policy, but relying on the traditional heuristic heavily limits the
expressivity of the policy class that these methods optimize
over, which prevents them from accurately approximating
Belady’s. In contrast, our work is the first to propose cache
replacement as an IL problem, which allows us to directly
train a replacement policy end-to-end over a much more expressive policy class to approximate Belady’s. This represents a novel way of leveraging Belady’s and provides a new
framework for learning end-to-end replacement policies.
Concretely, this paper makes the following contributions:
• We cast cache replacement as an imitation learning
problem, leveraging Belady’s in a new way (Section 3).
• We develop a neural architecture for end-to-end cache
replacement and several supervised tasks that further
improve its performance over standard IL (Section 4).
• Our proposed approach, PARROT, exceeds the state-ofthe-art replacement policy’s hit rates by over 20% on memory-intensive CPU benchmarks. On an industrialscale web search workload, PARROT improves cache
hit rates by 61% over a commonly implemented LRU
policy (Section 5).
• We propose cache replacement as a challenging new
IL/RL (reinforcement learning) benchmark involving
dynamically changing action spaces, delayed rewards,
and significant real-world impact. To that end, we
release an associated Gym environment (Section 7).
2. Cache Preliminaries
We begin with cache preliminaries before formulating cache
replacement as learning a policy over a Markov decision
process in Section 3. We describe the details relevant to
CPU caches, which we evaluate our approach on, but as
caching is a general concept, our approach can be extended
towards other cache structures as well.
A cache is a memory structure that maintains a portion of
the data from a larger memory. If the desired data is located
in the cache when it is required, this is advantageous, as
smaller memories are faster to access than larger memories.
Provided a memory structure, there is a question of how to
best organize it into a cache. In CPUs, caches operate in
terms of atomic blocks of memory or cache lines (typically
64-bytes large). This is the minimum granularity of data
that can be accessed from the cache.
During a memory access, the cache must be searched for
the requested data. Fully-associative caches layout all data
in a single flat structure, but this is generally prohibitively
expensive, as locating the requested data requires searching
through all data in the cache. Instead, CPU caches are often
W-way set-associative caches of size N × W, consisting
of N cache sets, where each cache set holds W cache lines
fl1; : : : ; lW g. Each line maps to a particular cache set (typically determined by the lower order bits of line’s address),
so only the W lines within that set must be searched.
During execution, programs read from and write to memory
addresses by executing load or store instructions. These
load/store instructions have unique identifiers known as
program counters (PCs). If the address is located in the
cache, this is called a cache hit. Otherwise, this is a cache
miss, and the data at that address must be retrieved from a
larger memory. Once the data is retrieved, it is generally
added to the appropriate cache set (as recently accessed
lines could be accessed again). Since each cache set can
only hold W lines, if a new line is added to a cache set
already containing W lines, the cache replacement policy
must choose an existing line to replace. This is called a
cache eviction and selecting the optimal line to evict is the
cache replacement problem.

<img src="https://latex.codecogs.com/svg.latex?\Large&space;x=\frac{-b\pm\sqrt{b^2-4ac}}{2a}" title="\Large x=\frac{-b\pm\sqrt{b^2-4ac}}{2a}" />
