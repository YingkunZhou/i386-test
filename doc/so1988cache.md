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

**Index Terms** - Cache, high-speed buffer, locality, memory
hierarchy, prefetching, program behavior, replacement algorithm,
trace-driven cache simulation, working set.

## INTRODUCTION

The gap in speed between a CPU and its memory in a computer system has persisted from one generation of
computers to another. Most likely, it will still continue at least
in conventional von Neumann machines, because a CPU and
its memory are drastically different in size and in technology
used. Although a CPU cannot be operating at its full speed
when its memory is far away, there are few programs that
really spread their data references all over the memory
locations continuously during its execution. This important
characteristic of program behavior is known as the principle
of program locality [3] and is the basis for a cache.

A cache is a high-speed buffer that is installed near a CPU to
hold the data (both program code and data) recently used by
the CPU. Usually it is only one or two CPU cycles away from
the CPU. If the CPU can find its data in the cache, it can
operate at its full speed most of the time.

Cache was first introduced in the IBM System/360 model 85
[8] with a size of only 16K bytes (1K = 1024). Since then, it
has been used not only in many high-end computers but also in
most minicomputers. With the advance in VLSI technology, a
limited scale of cache function has even been implemented in
some microcomputers. Caching is now considered as an
indispensable technique for a computer architect to speed up
the CPU(s) of a computer system.

A cache has three major ingredients: a data storage, a
directory, and a set of controls that implement a replacement
algorithm. The storage unit of a cache is usually called a line
which can hold a contiguous block of data from the main
line and the content of the line.

When a CPU accesses a line not in its cache, i.e., a cache
miss, another line may have to be replaced before the missed
line can be loaded. The most popular replacement algorithm is
the LRU (least recently used) algorithm, in which the
replacement of lines is governed by the order of the lines being
used and, by definition, the least recently used line is the
candidate for the next replacement. Therefore, the replacement
status of lines under an LRU replacement algorithm can
be represented by a stack. When a line is used by the CPU, it
becomes the MRU (most recently used) line and is placed at
the top of the stack; the line least recently used is at the bottom
of the stack.

For reason of tractability, most cache modeling and analysis
have been focused on fully associative caches in which all of
the lines arc placed on one stack. A model for a cache then
becomes very similar to a traditional memory model. As a
result, most of the analytical techniques for a memory are
readily applicable to a fully associative cache. However, in
order to minimize the number of comparisons between the
access address and the addresses of the lines in a cache, a
majority of computers today actually use set associative
caches. Lines in a set associative cache are organized in rows
and columns such that each row has its own replacement stack.
On a cache access, the address is hashed into one of the rows,
then it is compared associatively to all of the lines in the row to
determine a hit or a miss. This is why the rows are usually
called congruence classes and the number of the lines in each
row is called the set associativity, or simply the set size, of
the cache. A cache structure can therefore be determined by
the three parameters

$\text{number of congruence classes} \times \text{set associativity} \times \text{line size}$.

Although a fully associative cache can be viewed as a special
case of set associative caches, it is very difficult, if not
impossible, to relate the performance of one type of these
caches to the other.

From the viewpoint of a program running on a CPU, a very
important performance measure of a cache is the BHR (cache
or buffer hit ratio), which is defined by how often the CPU can
find its data in the cache. The complimentary measure BMR
(buffer miss ratio) is defined as 1 - BHR. Conventionally,
caches have simply been evaluated as black boxes; an analyst
varies the external parameters on one hand and collects the hit
ratios on the other.

The approach taken in this study is to take a closer look at
how a cache works, in a way that a cache storage is partitioned
into several regions, not by its physical structure, but by the
stack positions, i.e., the order of the lines that are used by the
CPU. Cache simulation with CPU traces has been used to
generate the cache hit distribution over these regions. It is not
difficult to realize that the distribution is higher towards the
lines that are recently used; our results show that, for
reasonable cache sizes, a substantially high percentage, about
90 percent, of the cache hits are concentrated on the MRU
lines. This a result of a strong relationship between the
working set of the programs being executed and the storage
structure of a set associative cache. The idea ofMRUchange,
which is based upon this observation, has two implications.
First, one can associate the set of MRU lines with the working
set of the CPU. Second, one can use the event when a CPU
accesses a non-MRU line in its cache to approximate the time
that the CPU is changing its working set. The scope of the
study is to support the idea with trace-driven simulations and
to explore the idea in many aspects of cache design and
performance evaluation.

In the next section, the performance of caches by replacement
status is presented. Because of the way a set associative
cache is addressed and the intensity of locality of computer
programs, cache simulation with our CPU traces shows that a
majority of the memory accesses are concentrated on the most
recently used lines and hence the MRU cache lines are highly
utilized. This suggests how and why one can detect a possible
shift of a working set of a CPU when a non-MRU line is used
in the cache.

In Section III, comparisons of the performance of several
partial LRU replacement algorithms are made. These algorithms
are characterized as tree replacement algorithms in a
sense that the replacement status of the lines can be represented
by trees. In particular, it is shown that, because the
memory accesses are so concentrated on the MRU lines, the
performance of these algorithms are surprisingly close to each
other for reasonable cache structures.

Prefetching of lines into a cache before they are really
needed has always been an interesting topic in the study of
caches. Probably this is due to its similarity to the prepaging of
memory system [1]. Section IV contains illustrations of using
the idea to improve the efficiency and coverage of any prefetch
algorithm.

The complexity of cache operations may not be the main
reason that makes trace-driven simulation of caches so
expensive and time consuming; it is the length of the traces. In
Section V, several techniques which use the MRU change to
reduce the cache simulation time are presented. Concluding
remarks and possible future work are in Section VI.

A good survey of cache memories can be found in [14].
Although a few terms used in this paper are slightly different
from the survey, they will be clearly defined before any use. A
reader should be aware that the paper is not intended to cover
all aspects of the cache topics mentioned above. Instead, its
emphasis is to illustrate how the idea of MRU change is
applied to each of the areas. To this respect, the simulation
data are provided only to support our points. The following
basic cache structure is assumed in this paper.
1. Cache organization: cache size: 64K bytes, line size: 128 bytes, set associativity: 4, and number of congruence classes: 128.
2. Replacement algorithm-LRU (least recently used).
3. Store policy-Store-in, or called write-back. A line must be loaded into a cache before it can be used by a CPU, and memory update for a store operation is deferred until the changed line is replaced from the cache. The policy
requires a change bit, also called dirty bit, per line in the
cache to indicate if a line has been modified by the CPU.
4. CPU access unit: double words (8 bytes).


This basic cache structure is essentially the one in an IBM
3081 system [7]. However, it is chosen only as a base for
comparison; one will find that our results are not limited to this
specific structure.

Three different CPU memory reference traces are used in
this study.

* TRACE1: Database-This is a trace of the Information
Management System [10] running on Multiple Virtual Storage
operating system [11].
* TRACE2: Scientific application-This is a uniprocessor
trace of the execution of fluid dynamics program [4]; but no
operating system trace is included.
* TRACE3: Batch jobs-This is the same as TRACEl
except that the system is running ten batch jobs, such as
sorting.

These are all IBM U370 memory reference traces of about
23 million instructions. They have been selected on the basis
that the underlying programs or systems have sufficiently
large working sets, the program initialization phrase had
already been completed, and the length guarantees that, for the
cache sizes studied, any cache initialization effects are
minimal.

## CACHEP ERFORMABNY CREE PLACEMESNTTA TUS

The most important function of a cache is to maintain, in a
high-speed buffer, the working set of the programs being
executed on a CPU. It is known from many empirical results
that a cache of 16K bytes can easily attain a BHR (buffer hit
ratio) of over 90 percent. A cache achieves this performance
by taking advantage of
* the programs’ temporal locality-the cache holds the
code and data recently used by the CPU
* the programs’ spatial locality-the cache fetches on every
miss an entire line from the main memory. Since a line
usually contains a multiple number of CPU access units,
subsequent accesses by the CPU to the data within the line tend
to be cache hits.

However, since a cache is built in a computer system very
close to a CPU, its access time and space are very critical. If
the memory is far away from the CPU, even one percent of
BHR can significantly affect the overall system performance.
It is therefore very important to see how the cache space is
utilized and where most of the cache accesses are spent. The
study of the distribution of cache hits over cache space is a step
further in understanding how the cache performs; it is intended
to provide a designer with effective time and space tradeoff.

From the viewpoint of performance modeling, while most
of the previous studies [9], [3] arranged the storage of a CPU
into multiple levels, such as cache, memory etc., our approach
is to partition the space within a cache itself.

Since the replacement algorithm of our basic cache structure
is LRU, the replacement status of a line in a congruence class
is defined by its current position on the LRU stack. For
example, each congruence class has a most recently used line
(top of the stack) and next-recently-used line (second on the
stack), etc. With this, the cache space of our basic cache
structure is viewed by every access as a storage of four
regions: REG, to REG4. A region REG, is a set of 128 lines;
each line is at the ith position of the LRU stack of a congruence
class.

The following measures are used in our cache simulations:
1) HIT_i is the number of times the CPU hits REG_i, 2) % HIT_i
is the percentage of hits to REG_i of all the hits to the cache, 3)
ENTER_i is the number of lines moved into REG_i, 4) the
utilization of REG_i is

UTIL_i = HIT_i/ENTER_i,

and 5) MISS is the set of accesses that cause cache misses.

The ENTER of each region can be calculated in the following way:

$ENTER1 = HIT2 + HIT3 + HIT4 + MISS$
$ENTER2 = ENTER, - 128$

and for $i \geq 2$,

$ENTERj+I=ENTERj-HITj- 128$.

UTIL, is an important measure, on a line basis, of temporal
locality of a program running on a CPU. For each region other
than REG1, UTILi can also be interpreted as the probability of
a line being rereferenced by the CPU when it was pushed to
the ith position of the stack. If the performance statistics are
collected after an initialization interval, i.e., warm start,
ENTER1 and ENTER2 are essentially the same.

The results of using traces TRACE1, TRACE2, and
TRACE3 to drive the basic cache structure are, respectively,
presented in Tables I, 11, and 111.

It is surprising that these three very different workloads
exhibit close performance in each replacement region, particularly
at REGl where all of the MRU lines are stored. Over 95
percent of the cache hits are concentrated on the MRU lines,
the access activities to the rest of cache space just fade out very
quickly. This observed performance is neither accidental nor
only specific to the workloads in the traces, but is principally a
result of the strong locality of the workloads and the way
accesses are hashed into a set associative cache.

The following is an explanation of how this can happen.
First note that the code and data of a program are packed in
pages at the memory level and in lines at the cache level (page
size and line size are, respectively, 4096 bytes and 128 bytes
in our model). When a line is referenced, it is loaded into the
MRU region and stays there until another line belonging to the
same congruence class is used. Since the line address of a
memory access is always hashed into one congruence class and
lines with consecutive addresses are hashed into consecutive
congruence classes, a block of memory tends to be loaded into
a region of the cache. More specifically, if the number of lines
in a page is less than the number of congruence classes in a
cache, all of the active lines in the page must be kept in the
MRU region as long as the page is actively used. As a result,
the lines in the MRU region are highly utilized and form a
“core” of the cache.

In our basic 64K cache, the MRU region has a capacity of
16K bytes (or four pages). Refer to Fig. 5 for an illustration;
when a program is being executed, its working set is kept in
the MRU region from time to time.

It is further claimed that, as long as the MRU of a cache is
large enough to hold the working set of a program, a similar
MRU performance can be obtained even if the cache structure
or size are varied. A series of cache simulations, with cache
sizes ranging from 4K to 256K bytes, have been carried out.
Fig. 1 shows that the cache hit distribution for TRACE1, and
particularly the rate to the MRU region, is over 80 percent for
a cache size of as little as 4K bytes. Fig. 2 shows the
distributions of all cache accesses, not just cache hits, to the
four regions. Figs. 3 and 4 are the corresponding plots for
TRACE2. For a regular cache of 32K bytes and larger, the
distributions of cache accesses and hits to the MRU region are
over 90 percent in TRACEl and are over 85 percent in
TRACE2. Similar results have been obtained for TRACE3.

Depending on page utilization, however, all lines in a page
are not necessarily utilized when a page is active. In general,
one expects that there are many pages active in the MRU
region, though exactly how many lines are used in the MRU
region depends on how strong is the sequentiality of an
application program.

The observation above shows how the working sets of
programs, the structures of set associative caches, and units of
memory segments, such as pages, are strongly related to each
other. Because the cache accesses are so concentrated on the
MRU region, it is possible to simplify the replacement status
of the lines in an n-way set associative cache into only three:
the MRU, the MED (medium), and the LRU. MRU and LRU
are defined as before, and MED contains the rest of the lines in
a stack.

The cache performance of the MRU region can be a very
good vehicle to study the working sets of programs running on
a CPU. If programs are packed in pages, and the MRU region
of a cache is large enough to hold a few pages, then the MRU
region is holding the working sets of the running programs
from time to time. Therefore, the CPU will be accessing the
MRU lines most of the time; once it references a line not in the
MRU region, very possibly it signals a change of its working
set. MRU change is an idea using a non-MRU access to
approximate a change of CPU working set.

The next few sections provide illustrations of applying the
idea of MRU change in several aspects of cache operations.

## REPLACEMENT ALGORITHMS CACHES

The choice and implementation of a replacement algorithm
in a set associative cache are important because the same
algorithm is used in the independently managed congruence
classes. Although the LRU replacement algorithm is not
optimal [2], it has been most widely studied through analytical
and simulation models. However, as a result of tradeoffs
between simplicity of implementation and performance, there
is a class of partial LRU replacement algorithms that are
actually implemented in real computers. A comparison of their
performance is presented in this section from the viewpoint of
MRU change.

First note that a stack that implements the LRU replacement
algorithm is a finite automaton with a number of states, each
represents an order of lines being used in a congruence class.
Since a stack of n lines has n! states, it requires at least
log2(n!) bits to specify all the states. This means, if the size of
a congruence class increases from n-way to (n + 1)-way, an
additional log2(n + 1) bits are needed to specify all the new
states. The complexity in structure and operation are the
reasons that caches with small sets are preferable. On the other
hand, since cache accesses are memory mapped, a cache with
a small set size usually generates a higher replacement rate for
useful lines. This can happen when many different lines
belonging to the same congruence class are actively used by
the CPU in a short period of time.

The above remains true even if the reduction in set
associativity is offset by a corresponding increase in the
number of congruence classes, i.e., the cache size remains
fixed. A cache of small set associativity is particularly
vulnerable in a multiprogramming environment in which a
CPU is usually running a variety of jobs in a short period of
time. For this performance reason, it is still desired to
maintain a sufficiently large set in any cache of a mainframe
computer.

Partial LRU replacement algorithms are introduced as a
compromise between implementation and performance. They
are derived from the following argument: since the chance that
a CPU hits the lines on the LRU stack of a congruence class
decreases significantly from the top to the bottom, it is
sufficient that a line to be replaced is not exactly the LRU line;
any line near to the bottom of the stack at that moment may be
adequate. If this is done without sacrificing too much of the
performance, there is no need to maintain an exact order of
reference of all the lines in a congruence class. Since a partial
LRU replacement algorithm maintains the order of line usage
as a partial ordering, rather than a total ordering, it can be
characterized as an algorithm with a replacement status
represented by a tree. Such a tree can be constructed with the
following procedure.
1) The lines in a congruence class are the leaves in the tree.
2) A group of lines have the same predecessor if they have
to obey either an LRU order or a random order for
replacement.
3) Each predecessor represents a group of lines and its
weight is the number of bits needed to represent the LRU
order. On the other hand, if a set of lines are randomly
replaceable, they have a predecessor of weight 0.
4) In the same way, a group of predecessors can have their
own predecessor.
5) A tree is formed when a common predecessor is reached
or all lines are covered, and the weight of the tree is the
number of bits required to implement the replacement
algorithm.

In the following examples, the first four are for four-way set
associative caches and the last one is for a 16-way set associate
cache.
1) The pure LRU algorithm shown in Fig. 6 is essentially a
flat tree.
2) Partitioned LRU (PLRU) algorithm is a balanced binary
tree shown in Fig. 7. It arranges the lines in a congruence
class in groups of two. In general, for a congruence class
of n lines, PLRU has 2(”-l) states and only needs n - 1
bits. This is the replacement algorithm used in IBM 3081
system.
3) The 1-bit replacement algorithm shown in Fig. 8 has two
states and requires only 1 bit per congruence class. Under
the scheme, all of the lines in a congruence class are
partitioned into two sides: one contains the MRU line and
the other does not. On a replacement, the cache control
keeps any lines on the MRU side, but replaces randomly
a line on the non-MRU side.
4) The random replacement algorithm is shown in Fig. 9.
5) The tree for the replacement algorithm of the 16-way set
associative cache in an IBM 3033 system is shown in Fig.
10.

Several simulations have been performed to see how the
above replacement algorithms are compared to each other. In
the simulation, the line size and the cache size are fixed at 128
bytes and 64K bytes, respectively, but the set associativity is
changing from 2 to 32. Figs. 11 and 12 are plots of ratios of
BMR (buffer miss ratio) of each partial LRU algorithm to that
of the LRU algorithm. These plots suggest that, for caches of
reasonable structures, the performance of any partial LRU
algorithm is close to that of the LRU algorithm. For example,
their performance difference is within 10 percent of BMR if
the set associativity is less than 8.

The results indicate that a set associative cache is very
robust in performance to any reasonable replacement algorithm.
The replacement status of the lines in each congruence
class can never be changed unless the CPU makes a non-
MRU access. If over 90 percent of cache accesses are directed
to the MRU region, 90 percent of the time these algorithms
agree with each other; therefore, no matter how differently
they can replace lines on cache misses, their performance
difference can at most be a few percent. Furthermore, when
the number of lines in a congruence class is taken into account,
the difference is reduced to the insignificant figures as shown.
This is also the idea behind the 1-bit replacement algorithm; it
guarantees that the current and prior MRU lines of each
congruence class are always in the cache.

The above comparison is true as long as the MRU region
constitutes a significant amount of space in the cache. As
shown in the figures, for unusually large set sizes, such as 16
or 32, the random and 1-bit algorithms can have relatively
poor performance.

The close approximation of the PLRU algorithm for all the
set sizes reveals that the number of bits used to approximate
the LRU algorithm must be proportional to the set size. In this
case, in order to satisfy certain performance requirements, the
1 -bit algorithm can be generalized to the n-bit algorithm for
large set sizes. As an example, if a cache has a set size of 16, a
“3-bit’’ algorithm can be used to approximate the LRU
algorithm. The “3-bit” algorithm puts every four lines in a
group and is operating the same way as the PLRU algorithm
for a set of four groups. On a replacement, one line is
randomly selected from the LRU group in the congruence
class.

## PREFETCHING

There are two popular performance measures of a prefetch
algorithm: accuracy measures the chance a CPU really uses a
prefetched line in a cache, and coverage tells the percentage of
cache misses really reduced by the prefetch algorithm.
However, there is a third measure that is very often overlooked
in an evaluation: how often is prefetching in operation.
This includes the time to collect prefetch information and to
initiate the prefetch mechanism, but does not include the time
taken to prefetch any line. For example, very often a prefetch
algorithm may find that a line to be prefetched is already in the
cache. This leads to the following measure of efficiency of a
prefetch algorithm:

efficiency
= number of lines prefetchedhumber of initiations.

Obviously the most effective way to increase the coverage of a
prefetch algorithm is to initiate the prefetch mechanism after
every cache access so that it can keep close track of how and
when every line in the cache is used. However, basing
prefetching on individual cache accesses is counterproductive
from two standpoints. First, the prefetch mechanism must be
very responsive since the cache is accessed almost every CPU
cycle. Second, the correct prefetch cannot be that different
between successive accesses to the same line.

At the other end of the event-trigger spectrum is to prefetch
on cache misses. The advantage of this scheme is its overhead
can be overlapped; but this limits the coverage to 50 percent
since a miss is required to perform a prefetch, which at most
can save a second miss. The scheme is particularly ineffective
when the CPU is experiencing a change of working set.

Using an MRU change as an event trigger fits nicely
between the two schemes above. When the accesses of a CPU
stay within the lines in the MRU region, i.e., its working set,
the activity is highly repetitious. It would be wasteful to
reinitiate any prefetch mechanism that was done at the time the
working set was being established. On the other hand, since an
MRU change signals a shift of the working set of a CPU, it is
the best time to start a prefetch mechanism.

Another advantage of using MRU change to initiate a
prefetch algorithm is to increase the coverage of a prefetch
algorithm. It was disclosed in [6] that prefetch on MRU
change can naturally create an important chaining effect, in
that it allows a prefetch algorithm to fetch in succession a set
of lines to the cache if the CPU really uses them one by one.
The idea is to load a prefetched line into the cache not with the
usual MRU status but with a non-MRU status, i.e., any status
in MED or LRU, for example, if lines 11, I*, and I3 were
detected by a prefetch algorithm that they were previously
used in succession and none of them are now resided in the
cache. When II is used again, it causes a miss; I, is prefetched
and loaded into the cache with a non-MRU status. Subsequently,
when l2 is really used, it causes an MRU change and
13 will be prefetched accordingly.

A series of cache simulations with the next-line prefetch
algorithm have been performed to support the above idea.
The next-line prefetch, also called one-block look-ahead
algorithm [14], is the simplest prefetch algorithm which bases
on no additional information; the address of a line to be
prefetched is implicit in the address of current access. In
particular, in our simulations only the next line prefetch of
instruction lines is considered, i.e., the prefetch mechanism is
initiated only when the CPU is making an instruction fetch. In
this case, the coverage is the percentage of instruction misses
reduced. Also, it is assumed in the simulations that a target
line is prefetched only if it is presently not in the cache and a
prefetched line is loaded in the cache with an LRU status.

The performance results in Figs. 13-15 are for TRACEl.
Similar results have been obtained for TRACE3. No simulation
of prefetching with TRACE2 is carried out since the trace
generates very few instruction misses. The figures are
generated with the number of congruence classes and set
associativity, respectively, fixed at 128 and 4, but the line size
is varying from 8 to 256 bytes. Obviously, prefetch on every
reference shows a sign of waste. For instance, it only
prefetches once out of every 35 initiations in the 64K cache.
Prefetch on MRU change has an efficiency much closer to that
of prefetch on cache miss and at the same time maintains a
coverage very close to that of prefetch on every reference.

Finally, from the implementation standpoint, prefetch on
MRU change is very easy to realize. For example, it does not
require any tag as the tagged prefetch that is disclosed in [5]
and well analyzed in [14].

## CACHES IMULATIONS

Simulation of cache organizations with CPU traces is very
time consuming, and yet there is always a need to perform
simulation with long traces. This section shows how the idea
of MRU change may be used to derive techniques to reduce
the simulation time.

### A. Directory Lookup

In a real computer, when a CPU access is presented at the
cache, it must quickly be determined if the accessed line is in
the cache or not. Most caches are built with parallel
comparators such that the line address of the access is
compared simultaneously to all of the line addresses, i.e.,
tags, in the associated congruence class. In a cache simulation,
however, the comparison is taken place in serial. With the idea
of MRU change, one can avoid this if the search always starts
with the MRU line in a congruence class. Obviously this is
achieved if an explicit stack is implemented in each congruence
class. However, the implementation of an explicit stack
needs a linked list. A simple alternative is to have a pointer for
each MRU line in the congruence classes; this immediately
creates an MRU fast path to the serial search. Under this
simple scheme, even any non-MRU access still uses the slow
and serial search, but most of the time it is operating on the
fast path. Therefore, its run-time is still close to any efficient
implementation. Note that the above search scheme is actually
applicable to a real computer in which parallel comparators
and parallel moving of cache lines are too expensive to be
realized.

### Various Cache Sizes

For a fixed memory reference trace, it is always desirable to
see how the performance changes with respect to a spectrum of
cache parameters. This can be done by varying one or more of
the three basic ingredients: line size, number of congruence
classes, and set size. The concept of MRU change not only can
help us understand how the cache performance is affected by
these parameters, but it can also help us generate the cache
performance of the smaller caches from that of the larger ones,
or vise versa. Consider the following four caches:
1) C1 16K bytes (128 x 1 x 128)
2) C2 32K bytes (128 x 2 x 128)
3) C3 48K bytes (128 x 3 x 128)
4) C4 64K bytes (128 x 4 x 128).

Let Hi and M;, respectively, be the set of all accesses which
cause a hit and a miss to cache C_i.

When all of them are driven by one reference trace, for i <
j , their cache performances are related by

1) $H_i  H_j$
2) $M_j  M_i$.

These known inclusion relations are actually how a stack
algorithm [9], [3] is defined, except that they are generalized
here for rows of stacks and are particularly useful for set
associative caches.

If one is interested in getting the performance of different
cache sizes with various set sizes, all he needs is to run the
cache structure with the largest set sizes that he wants.
Performance statistics of smaller cache sizes are obtainable in
the same simulation by keeping additional counters for each
stack position. For example, HI contains exactly the hits to the
MRU region of H4. With this, one can minimize the need to
perform cache simulations as often.

Another way of using the above inclusion relations is
derived from the opposite. Let us use the simulation of C, as
an example. The simulation can be broken down into two
steps. 1) Perform a simulation of C1 to generate H1 and M1,
and 2) use MI as a trace input to a three-way set associative
cache, say C, ‘ . The performance measures of C4 are obtained
by combining those of C1 and C3 ’ . Since HI can contribute to
over 90 percent of all the CPU references, MI, the set of
misses, is only a fraction of the original reference trace.
Therefore, it only takes a fraction of time to simulate C4.

By keeping HI and M,, one can obtain the performance
statistics of C,, C,, . in a much faster way. The idea is
called trace stripping and is explored in [12]. A similar
technique, called stack deletion, for fully associative caches is
described in [13]. These two techniques are identical when the
latter one is restricted to the top position, i.e., the MRU line,
of a replacement stack.

In general, a trace is stripped with respect to a set of
congruence classes and a specific line size. Such a trace can be
used to study a cache with another configuration, as long as all
the latter’s MRU changes are identifiable. That is, given two
caches C, C‘ and a trace that is stripped during the simulation
of C, the trace is usable to simulate C’ if, and only if, an MRU
change for C‘ is also an MRU change for C.

## CONCLUSIONS

A way of studying the internal performance of a cache is
presented in this paper. The study of the distributions of cache
accesses within the replacement regions in a cache generated
several interesting results. First, the cache accesses are heavily
concentrated in the MRU region. This is an important
characteristic of any set associative storage when its accesses
exhibit strong locality. Second, the concept of MRU change
can be used to trigger important cache events. When a running
program already has its working set loaded into the MRU
region, there must be a lot of repetitious cache operations;
otherwise the program does not have any locality at all. Any
cache mechanism should watch out for non-MRU accesses.

There are many aspects of cache design and performance
studied in this paper within the context of MRU change. First,
it was shown that a set associative cache is robust against
replacement algorithm variations. Second, the MRU change is
used to trigger prefetch algorithms. This reduces the burden of
cache control when it is running with a prefetch algorithm.
Finally, it is used to derive techniques to reduce the run-time
of cache simulations.

The concept of MRU change can be applied to many other
set associative structures or memory hierarchy in a computer
system. In a multiprocessor system in which each CPU has a
local cache, it is interesting to study the multicache consistency
problem or also known as XI (cross interrogation), from
the standpoint of MRU change. In the area of performance
modeling of computer systems, one can also use the concept to
characterize a workload on one hand, and to simplify a
performance model on the other.

### ACKNOWLEDGMENT

We would like to thank P. Minear and J. Peterson for
helping us in the development of the cache models for this
study. We also thank F. Giraud, J. Knight, L. Liu, J.
Pomerene, T. Puzak, P. Rosenfeld, and F. Sparacio for many
helpful discussions. We appreciate G. Pfister and H. Stone for
their numerous comments which helped to improve our
presentation. Finally we are indebted to G. Grohoski and J.
Hack for providing us with their fluid dynamics trace.
