# Data cache management using frequency-based replacement

## ABSTRACT

We propose a new frequency-based replacement algorithm for managing caches used for disk blocks by a file system, database management system, or disk control unit, which we refer to here as data caches. Previously, LRU replacement has usually been used for such caches. We describe a replacement algorithm based on the concept of maintaining reference counts in which locality has been “factored out”. In this algorithm replacement choices are made using a combination of reference frequency and block age. Simulation results based on traces of file system and I/O activity from actual systems show that this algorithm can offer up to 34% performance improvement over LRU replacement, where the improvement is expressed as the fraction of the performance gain achieved between LRU replacement and the theoretically optimal policy in which the reference string must be known in advance. Furthermore, the implementation complexity and efficiency of this algorithm is comparable to one using LRU replacement.