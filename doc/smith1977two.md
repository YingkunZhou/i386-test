# Two Methods for the Efficient Analysis of Memory Address Trace Data

https://ieeexplore.ieee.org/abstract/document/1702406

## Abstract:

The high cost of analyzing long memory address traces has limited most researchers to short traces and analysis algorithms that are linear in the length of the trace. We suggest two methods that permit a trace to be shortened in length by one to two orders of magnitude (or more) for later further analysis. The Stack Deletion Method eliminates all references in the trace to the top k levels of the LRU stack. The Snapshot Method records the reference bits of the pages in the original tape at discrete intervals and uses these bits to generate a new trace. Extensive measurements are presented, from which we conclude that there is little or no loss in accuracy using reduced traces for many purposes for a wide range of memory sizes and degrees of reduction.
