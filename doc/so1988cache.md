# Cache operations by MRU change

## Abstract

The performance of set associative caches is analyzed. The method used is to group the cache lines into regions
according to their positions in the replacement stacks of a cache,
and then to observe how the memory accesses of a CPU are
distributed over these regions. Results from our CPU traces show
that the memory accesses are heavily concentrated on the most
recently used (MRU) region in the cache. The concept of MRU
change is introduced; the idea is to use the event that the CPU
accesses a non-MRU line to approximate the time the CPU is
changing its working set. The concept is shown to be useful in
many aspects of cache design and performance evaluation, such
as comparison of various replacement algorithms, improvement
of prefetch algorithms, and speedup of cache simulation.
