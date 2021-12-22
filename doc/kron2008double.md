# Double-DIP: Augmenting DIP with Adaptive Promotion Policies to Manage Shared L2 Caches

In the 2nd Workshop on CMP Memory Systems and Interconnects (CMP-MSI), June 2008

## Abstract

In this paper, we study how the Dynamic Insert Policy (DIP) cache mechanism behaves in a multi-core shared-cache environment.
Based on our observations, we explore a new direction in the design space of caches called the promotion policy.
In a conventional LRU-based cache, a hit causes the line to be promoted to the MRU position in the recency stack.
Instead, we suggest an incremental promotion policy where each hit on a cacheline progressively moves it toward the MRU position.
We describe a generalization of the DIP approach that can simultaneously adapt both the insertion and promotion policies of a shared multi-core cache.
Our preliminary results indicate that promotion polices are a promising avenue to further improve the behavior of shared L2 caches.

## Introduction

Careful management of the on-chip cache resources has always been an important research topic for highperformance microprocessors.
Despite decades of work on cache optimizations, this continues to be a thriving research topic as evidenced by several recent studies on building better L2 caches [2, 7, 8, 10, 11].
The advent of multi-core processors has made L2 cache design more challenging due to the interactions and contention introduced by multiple cores competing for the same shared resources.
As a result, a substantial research effort has gone into finding new and better ways to manage multi-core shared caches [3, 4, 6, 9, 12].

In this paper, we start by examining Qureshi et al. 's recently proposed Dynamic Insertion Policy (DIP) cache management scheme [8].
The original evaluation of DIP was only in the context of a single-core processor.
We use simulations to study the impact of using this technique in a multi-core system.
Competition between cores for the shared L2 cache increases the overall L2 miss rate; the increase in cache activity seems like it might increase the number of opportunities for DIP to adapt to and provide more benefit.
This turns out to not be the case, and we provide a qualitative explanation for why this is so.
We then introduce a new design parameter for cache management called the promotion policy.
We then propose Double- DIP which is a generalization of Qureshi et al. 's DIP.
Our Double-DIP approach simultaneously considers both insertion and promotion policies, can dynamically adapt both of these, and also provides a finer level of granularity of adaptivity.

## Dynamic Insertion Policy (DIP)

In this section, we first review the original Dynamic Insertion Policy (DIP) for cache management.
We then briefly explain our simulation methodology and then present performance results for how DIP performs in both single-core and multi-core contexts.

### 2.1. Review of the Original DIP

Qureshi et al. observed that for some applications, there exist many cache lines that get brought into the cache and then are never used again before they are evicted [8].
Ideally, we would want to minimize the amount of time that these lines remain in the cache, as they are simply occupying space without providing any performance benefit (i.e., keeping them cached will not result in any additional cache hits).
The traditional LRU (least recently used) cache policy assumes that all newly installed cachelines are placed at the MRU (most recently used) position of the recency stack, which makes sense since this newly installed line is in fact the most recently used in the set.
For a cacheline with no reuse, however, such a policy ensures that this cacheline will remain in the cache for at least n more accesses to this set (for an n-way set-associative cache).
That is, it will require at least n cache misses to this set before this cacheline gets demoted to the LRU position and then gets evicted from the cache.

Qureshi et al. introduced the novel concept of an insertion policy.
That is, when the processor first installs a new cacheline into a cache set, the processor need not automatically place the line in the MRU position.
In particular, they first introduce the LRU Insertion Policy (LIP) which places newly installed cachelines in the LRU position of the recency stack.
If the line gets accessed again, then the cache promotes the line to the MRU position.
In some situations, the cacheline may be used again but not before another cacheline gets installed.
In this case, inserting in the LRU position will cause the line to get evicted and therefore cause an additional miss on its next access.
To combat this, Qureshi et al.
introduced the Bimodal Insertion Policy (BIP) that acts the same as LIP, except that a line may occasionally be inserted into the MRU position with some non-zero probability.

Finally, Qureshi et al. show that for some applications, BIP works really well, while for other applications, traditional MRU insertion is still the better option.
To deal with this diversity in behaviors, they proposed the Dynamic Insertion Policy (DIP) which tracks which of the two insertion policies performs better and then dynamically chooses the better approach.
The naive way to track which policy is performing better would be to implement two additional sets of "shadow tags."
Each set of shadow tags is responsible for simulating what the contents of the cache would have been had a particular insertion policy been used all along.
A small integer counter (called PSEL or the Policy Selector) can then be used to track which policy would have resulted in a higher number of cache misses.
This is all illustrated in Figure 1(a).
The main cache array uses this counter value to determine which insertion policy should be employed.

Full duplication of all of a cache's tags (plus LRU counters) would result in an unacceptably high overhead.
To estimate the performance of the two insertion policies with reasonable overhead, Qureshi et al.
propose a novel monitoring mechanism called set dueling.
The idea is to dedicate a few sets in the cache to always employ a single insertion policy.
Figure 1(b) shows a figure depicting the sets in a cache.
The lightly shaded sets always make use of a traditional MRU insertion policy (i.e., a true traditional LRU replacement policy); the darker shaded sets always make use of the Bimodal insertion policy.
A miss in a LRU set causes the PSEL counter to be incremented, while a miss in a BIP set causes PSEL to be decremented.
This approach effectively simulates the different insertion policies in vivo within the cache, and then extrapolates the overall benefit of the two insertion policies based on these "leader" sets.
As a result, the hardware overhead is reduced to the trivial amount required to implement the single PSEL counter.
They also describe a simple complement-select mechanism for identifying which sets are leader sets without storing any extra information.

## Experimental Methodology

We make use of simulation to generate all of our experimental results.
In particular, we use the pre-release version of the x86 SimpleScalar toolset [1].
Our cycle-level simulator includes detailed models of all of the caches, the intercache buses, miss-status handling registers (MSHRs), offchip bus (FSB) contention and memory-controller queuing delays.
Our baseline processor configuration is based on the Intel Core 2 Duo (65nm/Merom version); Table 1 lists the microarchitecture details.
We make use of programs from the SPECcpu 2006 suite.
In particular, we selected benchmarks with varying L2 cache miss rates.
In this fashion, we could explore the impact of having two co-scheduled programs with different L2 cache access/miss rates and see how that impacts overall cache behavior and system performance.
For each benchmark, we make use of the Sim- Point 3.2 toolkit to select representative samples [5].
For the single-core runs, we first fast-forward while warming all caches for 500 million instructions, and then perform detailed cycle-level simulation for 100 million committed instructions.
For the multi-core runs, we fast-forward by 500 million instructions per core in a round-robin fashion, and then perform detailed simulation for 100 million instructions per core.
When one core reaches its instruction limit, we freeze its relevant statistics, but we then continue to simulate its execution so that it still competes with the other core for shared resources.
This is similar to previous methodologies employed in multi-core cache studies [9].

## DIP Performance

We evaluated the performance benefit of DIP for both single-core and multi-core configurations.
Figure 2 shows the L2 miss rates (measured in misses per thousand instructions or MPKI) and the IPC improvements of our benchmarks for both a conventional LRU policy (i.e., MRU insertion) and the DIP policy.
Overall, DIP reduces the L2 miss rate and increases performance.
The magnitude of the benefits are not as large as previously reported, but this is primarily due to the fact that our simulation makes use of a larger and more highly set-associative contemporary L2 cache organization (4MB/16-way, like that used in the 65nm Intel Core 2 Duo), as opposed to the 1MB/8-way L2 used in the original DIP study.

Figure 3 shows the results for our dual-core workloads.
For the DL2 miss rates, we report the average between the two programs.
For the IPC improvements, we use both the increase in the total IPC throughput and the weighted speedup metrics [13]; details are listed in Table 2.
Compared to the single-core runs, the number of L2 misses has increased quite dramatically.
This makes sense since the two cores now have to compete with each other for the shared cache resources.
With so many more misses, one might expect DIP to perform better than in the singlecore case because it has more dynamic behavior to attempt to adapt over (i.e., there should be more opportunities for adaptation).
The results, however, show that the relative benefit of DIP is still about the same as in the single-core case (this can still be viewed as a positive result that says that going to multi-core shared caches does not break DIP).

## Double-DIP

### 5.1. Cache Promotion Policies

For the BIP component of DIP, most cachelines will be inserted at the LRU position of the recency stack.
On a subsequent hit, the line will then be promoted to the MRU position.
In a dual-core situation, the previously most-recentlyused line may belong to the other core.
Now consider the example shown in Figure 4(a) where one core (P0) accesses one set in the cache more frequently than the other (P1).
The multiple accesses by P0 rapidly push all of P1's cachelines down to the less-recently used end of the LRU recency stack, leaving them far more vulnerable to eviction.
When combined with insertion at the LRU position (LIP/BIP/DIP), P1 will have a very difficult time trying to keep its data in the cache.

To help mitigate the pathological access patterns described above, we propose a new design parameter for cache management: the cache promotion policy.
A traditional cache uses a MRU promotion policy meaning that anytime there is a cache hit, the line is instantly promoted to the MRU position in the recency stack.
One possible alternative promotion policy is to incrementally move the cache line toward the MRU position(<sub>We should technically not be calling the "leftmost" position (with respect to the recency stack) the most-recently used as it no longer holds the most recently used line, but we will still refer to the leftmost position as MRU to avoid introducing new terminology.
This is also effectively consistent with the terminology used in the original DIP paper.</sub>).
For example, Figure 4(b) shows the same access sequence, but on a hit, we only move the cacheline by one position to the left.
As a result, it takes several more accesses for a cacheline to find its way to the MRU position in the recency stack.
The net result is that the positions of P1's cachelines are able to stay further to the left in the recency stack, which means that P1's cachelines are, on average, in less danger of eviction than when we used the conventional MRU-promotion policy.
This helps to level the playing field between programs with different L2 cache access frequencies.
Based on this intuition, we explore the potential for better promotion policies.

### 5.2. Double-DIP: Combining Cache Insertion and Promotion Policies

We propose to augment the original DIP approach to handle cache promotion policies different than the conventional MRU promotion approach.
In particular, we make use of Qureshi et al. 's set-dueling approach to monitor the potential for two different promotion policies.
The first promotion policy is the conventional MRU approach.
The second policy is the Single-step Incremental Promotion Policy (SIPP), that was illustrated in Figure 4(b).
Based on the misses observed in the leader sets, we use a PSEL counter to determine the promotion policy employed in the remaining follower sets.
Together, this implements a Dynamic Promotion Policy (DPP).

We actually implement both DPP and DIP at the same time.
This requires that we make use of twice as many leader sets: half of the sets track the component insertion policies, and the other half of the sets track the component promotion policies.
We make use of two PSEL counters as well: IPSEL (insertion policy selector) and PPSEL (promotion policy selector).
This slightly increases the hardware overhead (from one small counter to two), but this is still a very small amount of additional state.
Note that insertion and promotion are orthogonal design considerations.
The insertion policy is only invoked on the installation of a new cacheline, and the promotion policy is only considered when hitting on an already cached line.
As a result, the leader sets for the promotion policies are actually follower sets with respect to the dynamic insertion policy.
Similarly, the insertion policy leader sets are follower sets for the dynamic promotion policy.

Our leader set allocation method is a simple extension to the original DIP complement-select strategy.
In complement-select, the N sets in the cache are divided into K constituencies, where each constituency has one leader set per policy.
The most significant <img src="https://render.githubusercontent.com/render/math?math=k = log_2 K"> bits of the set index determine the constituency number.
The leader set for the first policy is that set where the least significant k bits of the set index are equal to the constituency number.
That is, the mth set of constituency number m is the leader set for the first policy.
For the second policy, the leader set is chosen by the set whose k least significant bits are equal to the bitwise complement of the constituency number.
Figure 5(a) shows an example of the leader sets chosen by complement select.
In our approach, we take the k+1 least significant bits and right-shift this number by one position (equivalent to taking k bits after omitting the least significant bit), and then compare this to the constituency number (or its complement).
We then make use of the remaining least significant bit to determine whether the set is a leader for the insertion policy or the promotion policy.
Figure 5(b) illustrates an example.
The original DIP policy recommended using at least 16 leader sets per policy; our experiments confirmed this is to be a good number.

We initially had our L2 cache use set-dueling to choose between an MRU Promotion Policy (MPP) and the Singlestep Incremental Promotion Policy (SIPP).
For some workloads, this proved to be an effective approach, but for others the results were not as good.
The main problem is that even with dynamic adaption, direct promotion to the MRU position is still frequently too aggressive and can cause bad shared cache interactions.
As a result, we propose the Dynamic Promotion with Interpolated Increments Policy (D-PIIP) that can choose different promotion increment amounts based on the strength of the PPSEL counter.
Normally, we would use the most significant bit of PPSEL to effectively choose between a promotion of +1 position (SIPP) versus a promotion of +n positions (MPP; n is the set-associativity).
With D-PIIP, we use the two most significant bits to select promotion increments of +1, +2, +4 and +n.
Figure 6 shows the final overall organization of the Double-DIP scheme.

Due to the strong resemblance of our DPP (and the derivative D-PIIP) scheme to the original DIP approach, we call the overall combined scheme "Double-DIP" to properly acknowledge the genesis of our proposal.

### 5.3. Double-DIP Results

We evaluated the performance of Double-DIP on our dualcore workloads.
Figure 7 shows the L2 MPKI and the IPC performance improvements using conventional LRU as the baseline.
For reference, the original DIP approach, as well as DIP/SIPP (adaptive promotion between +1 and +n only) are also included.
For DIP/SIPP, we can see that there are a few cases where the promotion policy provides some striking reductions in miss rates (e.g., astar(bl)::milc and astar(bl)::soplex(ref)), but for many other situations it performs even worse than the baseline LRU policy.
When we make use of Double-DIP's variable promotion increments, the overall average miss-rate improves even further, and the improvements across the workloads is far more consistent (i.e., there are fewer workloads where Double-DIP performs worse than original DIP or LRU).

The performance results are similar for both weighted IPC and throughput metrics.
The DIP/SIPP approach achieves strong speedups on only three of the workloads.
Even worse, DIP/SIPP causes a performance slowdown on a substantial number of workloads.
The Double-DIP approach works far better than both the original DIP and the DIP/SIPP schemes.
The only outlier is the bzip2(c)::mcf workload where we suffer a 8.6% (9.2%) reduction in performance as measured by the IPC weighted speedup (throughput).
There are many potential reasons for the performance anomalies of DIP/SIPP and Double-DIP.
This could be due to the cross-interactions/contamination between the two sets of leader sets for insertion and promotion policies.
Another possibility is that our simple extension to the complement-select leader-set allocation scheme may have introduced some unexpected interference.
Further analysis is required to fully understand the interactions between insertion policies, promotion policies, and how the access patterns of the multi-core workload all affect each other.
Such work will be explored in the future, but these preliminary results are very encouraging and strongly suggest that the cache promotion policy is a new dimension in the cache design space that warrants further investigation.

The performance benefits of our Double-DIP scheme appear to be unique to multi-core shared-cache scenarios.
We ran additional experiments where we make use of Double- DIP with single-program workloads, and the overall performance was very similar to that of the original DIP approach.
We believe that this provides evidence to back up our hypothesis that Double-DIP helps deal with the recency-stack interference caused by two programs with different L2 access rates fighting over the shared cache resources.

The results in Figure 7 also show that the original DIP has much more stable behavior.
That is, when DIP works, it works; when it is not beneficial, it does not have much negative impact on the workloads either.
Double-DIP on the other hand works very well in some cases, but it still causes some relatively significant performance decreases on certain workloads.
Our current implementation of Double- DIP has not been extensively tuned and optimized, and we are confident that with some furhter tweaks, we can minimize the cases where Double-DIP hurts performance.
One straightforward adjustment is to simply change the promotion increments to have more MRU-promotion-like behavior (e.g., use increments of +4/+8/+12/+16).
Another possible modification is provide per-core PSEL counters.
The idea is that on a miss in a leader set, we also check to see which core initiated the cache access and then update its corresponding PSEL counter.
For a dual-core configuration, this would add up to four PSEL counters (one PPSEL and one IPSEL, per core).
In this fashion, each core can adopt different levels of aggressiveness with respect to both insertion and promotion policies.

## Conclusions

In this work, we have introduced the concept of a cache's promotion policy as a new design parameter.
Our preliminary results indicate that this is a promising avenue for improving the behavior of caches in a multi-core system.
The Double-DIP approach presented in this paper is only a firststep in exploring ways to exploit different promotion policies for shared caches.
Future research directions include optimization and refinement of the Double-DIP scheme.
The idea of interpolated increments can potentially be applied to insertion policies as well, which may work for both single- and multi-core (including more than two cores) scenarios.
Additional research is requried for evaluating these techniques with multi-threaded workloads (as opposed to multi-programmed) where two or more threads may share some cache lines.
For all scenarios, fairness also needs to be studied, and the policies may need further refinements to strike the right balance between fairness and performance.
As mentioned earlier, much more work is required to analyze promotion policies and understand why and how they work.
Most modern caches do not make use of true LRU replacement policies due to the complexity in updating all of the recency counters, especially for modern highly-associative caches.
It may be fruitful to explore how to adapt promotion policies to LRU approximation algorithms such as NMRU and pseudo-LRU replacement policies.

### Acknowledgments

Funding and equipment were provided by a grant from Intel Corporation.
