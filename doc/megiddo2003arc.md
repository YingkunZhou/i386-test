# ARC: A SELF-TUNING, LOW OVERHEAD REPLACEMENT CACHE

## Abstract

We consider the problem of cache management
in a demand paging scenario with uniform page sizes. We
propose a new cache management policy, namely, Adaptive
Replacement Cache (ARC), that has several advantages.
In response to evolving and changing access patterns, ARC
dynamically, adaptively, and continually balances between the
recency and frequency components in an online and self-
tuning fashion. The policy ARC uses a learning rule to
adaptively and continually revise its assumptions about the
workload.
The policy ARC is empirically universal, that is, it empir-
ically performs as well as a certain fixed replacement policy–
even when the latter uses the best workload-specific tuning
parameter that was selected in an offline fashion. Conse-
quently, ARC works uniformly well across varied workloads
and cache sizes without any need for workload specific a
priori knowledge or tuning. Various policies such as LRU-2,
2Q, LRFU, and LIRS require user-defined parameters, and,
unfortunately, no single choice works uniformly well across
different workloads and cache sizes.
The policy ARC is simple-to-implement and, like LRU,
has constant complexity per request. In comparison, policies
LRU-2 and LRFU both require logarithmic time complexity
in the cache size.
The policy ARC is scan-resistant: it allows one-time se-
quential requests to pass through without polluting the cache.
On 23 real-life traces drawn from numerous domains, ARC
leads to substantial performance gains over LRU for a wide
range of cache sizes. For example, for a SPC1 like synthetic
benchmark, at 4GB cache, LRU delivers a hit ratio of 9.19%
while ARC achieves a hit ratio of 20%.
