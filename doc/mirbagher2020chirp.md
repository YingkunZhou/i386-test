# CHiRP: Control-Flow History Reuse Prediction

https://ieeexplore.ieee.org/abstract/document/9251943

## Abstract:

Translation Lookaside Buffers (TLBs) play a critical role in hardware-supported memory virtualization. To speed up address translation and reduce costly page table walks, TLBs cache a small number of recently-used virtual-to-physical address translations. TLBs must make the best use of their limited capacities. Thus, TLB entries with low potential for reuse should be replaced by more useful entries. This paper contributes to an aspect of TLB management that has received little attention in the literature: replacement policy. We show how predictive replacement policies can be tailored toward TLBs to reduce miss rates and improve overall performance. We begin by applying recently proposed predictive cache replacement policies to the TLB. We show these policies do not work well without considering specific TLB behavior. Next, we introduce a novel TLB-focused predictive policy, Control-flow History Reuse Prediction (CHIRP). This policy uses a history signature and replacement algorithm that correlates to known TLB behavior, outperforming other policies. For a 1024-entry 8-way set-associative L2 TLB with a 4KB page size, we show that CHiRP reduces misses per 1000 instructions (MPKI) by an average 28.21% over the least-recently-used (LRU) policy, outperforming Static Re-reference Interval Prediction (SRRIP) [1], Global History Reuse Policy (GHRP) [2] and SHiP [3], which reduce MPKI by an average of 10.36%, 9.03% and 0.88%, respectively.