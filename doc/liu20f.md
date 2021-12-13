# An Imitation Learning Approach for Cache Replacement

https://proceedings.mlr.press/v119/liu20f.html

## Abstract

Program execution speed critically depends on increasing cache hits, as cache hits are orders of magnitude faster than misses.
To increase cache hits, we focus on the problem of cache replacement: choosing which cache line to evict upon inserting a new line.
This is challenging because it requires planning far ahead and currently there is no known practical solution.
As a result, current replacement policies typically resort to heuristics designed for specific common access patterns, which fail on more diverse and complex access patterns.
In contrast, we propose an imitation learning approach to automatically learn cache access patterns by leveraging Belady's, an oracle policy that computes the optimal eviction decision given the future cache accesses.
While directly applying Belady's is infeasible since the future is unknown, we train a policy conditioned only on past accesses that accurately approximates Belady's even on diverse and complex access patterns, and call this approach Parrot.
When evaluated on 13 of the most memory-intensive SPEC applications, Parrot increases cache miss rates by 20% over the current state of the art.
In addition, on a large-scale web search benchmark, Parrot increases cache hit rates by 61% over a conventional LRU policy.
We release a Gym environment to facilitate research in this area, as data is plentiful, and further advancements can have significant real-world impact.

## Introduction

Caching is a universal concept in computer systems that bridges the performance gap between different levels of data storage hierarchies, found everywhere from databases to operating systems to CPUs (Jouppi, 1990; Harty & Cheriton, 1992; Xu et al., 2013; Cidon et al., 2016).
Correctly selecting what data is stored in caches is critical for latency, as accessing the data directly from the cache (a cache hit) is orders of magnitude faster than retrieving the data from a lower level in the storage hierarchy (a cache miss).
For example, Cidon et al. (2016) show that improving cache hit rates of web-scale applications by just 1% can decrease total latency by as much as 35%.

Thus, general techniques for increasing cache hit rates would significantly improve performance at all levels of the software stack.
Broadly, two main avenues for increasing cache hit rates exist: (i) avoiding future cache misses by proactively prefetching the appropriate data into the cache beforehand; and (ii) strategically selecting which data to evict from the cache when making space for new data (cache replacement).
Simply increasing cache sizes is a tempting third avenue, but is generally prohibitively expensive.

This work focuses on single-level cache replacement (Figure 1).
When a new block of data (referred to as a line) is added to the cache (i.e., due to a cache miss), an existing cache line must be evicted from the cache to make space for the new line.
To do this, during cache misses, a cache replacement policy takes as inputs the currently accessed line and the lines in the cache and outputs which of the cache lines to evict.

Prior work frequently relies on manually-engineered heuristics to capture the most common cache access patterns, such as evicting the most recently used (MRU) or least recently used (LRU) cache lines, or trying to identify the cache lines that are cache-friendly vs. cache-averse (Qureshi et al., 2007; Jaleel et al., 2010; Jain & Lin, 2016; Shi et al., 2019).
These heuristics perform well on the specific simple access patterns they target, but they only target a small fraction of all possible access patterns, and consequently they perform poorly on programs with more diverse and complex access patterns.
Current cache replacement policies resort to heuristics as practical theoretical foundations have not yet been developed (Beckmann & Sanchez, 2017).

We propose a new approach for learning cache replacement policies by leveraging Belady's optimal policy (Belady, 1966) in the framework of imitation learning (IL), and name this approach PARROT.
(<sub>Parrots are known for their ability to imitate others.</sub>)
Belady's optimal policy (Belady's for short) is an oracle policy that computes the theoretically optimal cache eviction decision based on knowledge of future cache accesses, which we propose to approximate with a policy that only conditions on the past accesses.
While our main goal is to establish (imitation) learned replacement policies as a proof-of-concept, we note that deploying such learned policies requires solving practical challenges, e.g., model latency may overshadow gains due to better cache replacement.
We address some of these challenges in Section 4.5 and highlight promising future directions in Section 7.

Hawkeye (Jain & Lin, 2016) and Glider (Shi et al., 2019) were the first to propose learning from Belady's.
They train a binary classifier to predict if a cache line will soon be reused (cache-friendly) or not (cache-averse), evicting the cache-averse lines before the cache-friendly ones and relying on a traditional heuristic to determine which lines are evicted first within the cache-friendly and cache-averse groups.
Training such a binary classifier avoids the challenges (e.g., compounding errors) of directly learning a policy, but relying on the traditional heuristic heavily limits the expressivity of the policy class that these methods optimize over, which prevents them from accurately approximating Belady's.
In contrast, our work is the first to propose cache replacement as an IL problem, which allows us to directly train a replacement policy end-to-end over a much more expressive policy class to approximate Belady's.
This represents a novel way of leveraging Belady's and provides a new framework for learning end-to-end replacement policies.

Concretely, this paper makes the following contributions:
- We cast cache replacement as an imitation learning problem, leveraging Belady's in a new way (Section 3).
- We develop a neural architecture for end-to-end cache replacement and several supervised tasks that further improve its performance over standard IL (Section 4).
- Our proposed approach, PARROT, exceeds the state-ofthe-art replacement policy's hit rates by over 20% on memory-intensive CPU benchmarks. On an industrialscale web search workload, PARROT improves cache hit rates by 61% over a commonly implemented LRU policy (Section 5).
- We propose cache replacement as a challenging new IL/RL (reinforcement learning) benchmark involving dynamically changing action spaces, delayed rewards, and significant real-world impact. To that end, we release an associated Gym environment (Section 7).

## Cache Preliminaries

We begin with cache preliminaries before formulating cache replacement as learning a policy over a Markov decision process in Section 3.
We describe the details relevant to CPU caches, which we evaluate our approach on, but as caching is a general concept, our approach can be extended towards other cache structures as well.

A cache is a memory structure that maintains a portion of the data from a larger memory.
If the desired data is located in the cache when it is required, this is advantageous, as smaller memories are faster to access than larger memories.
Provided a memory structure, there is a question of how to best organize it into a cache.
In CPUs, caches operate in terms of atomic blocks of memory or cache lines (typically 64-bytes large).
This is the minimum granularity of data that can be accessed from the cache.

During a memory access, the cache must be searched for the requested data.
Fully-associative caches layout all data in a single flat structure, but this is generally prohibitively expensive, as locating the requested data requires searching through all data in the cache.
Instead, CPU caches are often W-way set-associative caches of size <img src="https://render.githubusercontent.com/render/math?math=N \times W"> consisting of <img src="https://render.githubusercontent.com/render/math?math=N"> cache sets, where each cache set holds <img src="https://render.githubusercontent.com/render/math?math=W"> cache lines <img src="https://render.githubusercontent.com/render/math?math=\{l_1, \dots, l_w\}"> Each line maps to a particular cache set (typically determined by the lower order bits of line's address), so only the <img src="https://render.githubusercontent.com/render/math?math=W"> lines within that set must be searched.

During execution, programs read from and write to *memory addresses* by executing load or store instructions.
These load/store instructions have unique identifiers known as *program counters* (PCs).
If the address is located in the cache, this is called a *cache hit*.
Otherwise, this is a *cache miss*, and the data at that address must be retrieved from a larger memory.
Once the data is retrieved, it is generally added to the appropriate cache set (as recently accessed lines could be accessed again).
Since each cache set can only hold <img src="https://render.githubusercontent.com/render/math?math=W"> lines, if a new line is added to a cache set already containing <img src="https://render.githubusercontent.com/render/math?math=W"> lines, the cache replacement policy must choose an existing line to replace.
This is called a *cache eviction* and selecting the optimal line to evict is the cache replacement problem.

**Belady's Optimal Policy**.
Given knowledge of future cache accesses, Belady's computes the optimal cache eviction decision.
Specifically, at each timestep <img src="https://render.githubusercontent.com/render/math?math=\large t"> Belady's computes the reuse distance <img src="https://render.githubusercontent.com/render/math?math=\large d_t(l_w)"> for each line <img src="https://render.githubusercontent.com/render/math?math=\large l_w"> in the cache set, which is defined as the number of total cache accesses until the next access to <img src="https://render.githubusercontent.com/render/math?math=\large l_w"> Then, Belady's chooses to evict the line with the highest reuse distance, effectively the line used furthest in the future, i.e., <img src="https://render.githubusercontent.com/render/math?math=\large arg\ max_{w = 1,\dots , W} d_t(l_w)">.

## Casting Cache Replacement as Imitation Learning

We cast cache replacement as learning a policy on an episodic Markov decision process <img src="https://render.githubusercontent.com/render/math?math=\langle \mathcal{S}, \mathcal{A}_s, R, P \rangle"> in order to leverage techniques from imitation learning.
Specifically, the state at the t-th timestep <img src="https://render.githubusercontent.com/render/math?math=\Large s_t = (s^c_t, s^a_t, s^h_t) \in \mathcal{S}"> consists of three components, where:
- <img src="https://render.githubusercontent.com/render/math?math=\Large s^a_t = (m_t, pc_t)"> is the current cache access, consisting of the currently accessed cache line address <img src="https://render.githubusercontent.com/render/math?math=m_t"> and the unique program counter <img src="https://render.githubusercontent.com/render/math?math=pc_t"> of the access.
- <img src="https://render.githubusercontent.com/render/math?math=\Large s^c_t = \{l_1, \dots , l_W\}"> is the cache state consisting of the <img src="https://render.githubusercontent.com/render/math?math=W"> cache line addresses currently in the cache set accessed by <img src="https://render.githubusercontent.com/render/math?math=\Large s^a_t"> (the replacement policy does not require the whole cache state including other cache sets to make a decision).(<sub>A cache set can have less than <img src="https://render.githubusercontent.com/render/math?math=W"> cache lines for the first <img src="https://render.githubusercontent.com/render/math?math=W-1"> cache accesses (small fraction of program execution). In this case, no eviction is needed to insert the line.</sub>)
- <img src="https://render.githubusercontent.com/render/math?math=\Large s^h_t = \{m_1, \dots , m_{t-1}\},\{pc_1, \dots , pc_{t-1}\}"> is the history of all past cache accesses. In practice, we effectively only condition on the past <img src="https://render.githubusercontent.com/render/math?math=H"> accesses.

The action set <img src="https://render.githubusercontent.com/render/math?math=\mathcal{A}_{s_t}"> available at a state <img src="https://render.githubusercontent.com/render/math?math=\Large s_t = (s^c_t, s^a_t, s^h_t)"> is defined as follows: During cache misses, i.e., <img src="https://render.githubusercontent.com/render/math?math=m_t \notin s^c_t">, the action set <img src="https://render.githubusercontent.com/render/math?math=\mathcal{A}_{s_t}"> consists of the integers <img src="https://render.githubusercontent.com/render/math?math=\{1, \dots, W\}">, where action <img src="https://render.githubusercontent.com/render/math?math=w"> corresponds to evicting line <img src="https://render.githubusercontent.com/render/math?math=l_w">.
Otherwise, during cache hits, the action set <img src="https://render.githubusercontent.com/render/math?math=\mathcal{A}_{s_t}"> consists of a single no-op action <img src="https://render.githubusercontent.com/render/math?math=a_{no-op}">, since no line must be evicted.

The transition dynamics <img src="https://render.githubusercontent.com/render/math?math=P(s_{t+\pm 1} | a_t, s_t)"> are given by the dynamics of the three parts of the state.
The dynamics of the next cache access <img src="https://render.githubusercontent.com/render/math?math=s^a_{t+\pm 1}"> and the cache access history <img src="https://render.githubusercontent.com/render/math?math=s^h_{t+\pm 1}"> are independent of the action <img src="https://render.githubusercontent.com/render/math?math=a_t"> and are defined by the program being executed.
Specifically, the next access <img src="https://render.githubusercontent.com/render/math?math=s^a_{t+\pm 1} = (m_{t+\pm 1}, pc_{t+\pm 1})"> is simply the next memory address the program accesses and its associated PC.
The t-th access is appended to <img src="https://render.githubusercontent.com/render/math?math=s^h_{t+\pm 1}">, i.e., <img src="https://render.githubusercontent.com/render/math?math=s^h_{t+\pm 1} = (\{m1, \dots, m_{t-1}, m_t\}, \{pc1, \dots, pc_{t-1}, pc_t\})">.

The dynamics of the cache state are determined by the actions taken by the replacement policy.
At state <img src="https://render.githubusercontent.com/render/math?math=s_t"> with <img src="https://render.githubusercontent.com/render/math?math=s^c_t = \{l_1, \dots, l_W\}"> and <img src="https://render.githubusercontent.com/render/math?math=s^a_t = (m_t, pc_t)">: A cache hit does not change the cache state, i.e., <img src="https://render.githubusercontent.com/render/math?math=s^c_{t+\pm 1} = s^c_t">, as the accessed line is already available in the cache.
A cache miss replaces the selected line with the newly accessed line, i.e., <img src="https://render.githubusercontent.com/render/math?math=s^c_{t+\pm 1} = \{l_1, \dots l_{w-1}, l_{w+\pm 1}, \dots, l_W, m_t\}"> where <img src="https://render.githubusercontent.com/render/math?math=a_t = w">.

The reward <img src="https://render.githubusercontent.com/render/math?math=R(s_t)"> is 0 for a cache miss (i.e., <img src="https://render.githubusercontent.com/render/math?math=m_t \notin s^c_t">) and is 1 otherwise for a cache hit.
The goal is to learn a policy <img src="https://render.githubusercontent.com/render/math?math=\pi_\theta(a_t | s_t)"> that maximizes the undiscounted total number of cache hits (the reward), <img src="https://render.githubusercontent.com/render/math?math=\sum^T_{t = 0} R(s_t)">, for a sequence of <img src="https://render.githubusercontent.com/render/math?math=T"> cache accesses <img src="https://render.githubusercontent.com/render/math?math=(m_1, pc_1), \dots, (m_T, pc_T)">.

In this paper, we formulate this task as an imitation learning problem.
During training, we can compute the optimal policy (Belady's) <img src="https://render.githubusercontent.com/render/math?math=\pi^*(a_t | s_t, (m_{t+\pm 1}, pc_{t+\pm 1}, \dots , (m_T, pc_T))">, by leveraging that the future accesses are fixed.
Then, our approach learns a policy <img src="https://render.githubusercontent.com/render/math?math=\pi_\theta (a_t | s_t)"> to approximate the optimal policy without using the future accesses, as future accesses are unknown during test time.

To demonstrate the difficulty of the problem, similar to the figure from Jain & Lin (2016), Figure 2 shows the amount of future information required to match the performance of Belady's on a common computer architecture benchmark (omnetpp, Section 5).
We compute this by imposing a future window of size <img src="https://render.githubusercontent.com/render/math?math=x"> on Belady's, which we call <img src="https://render.githubusercontent.com/render/math?math=Belady_x">, Within the window (reuse distances <img src="https://render.githubusercontent.com/render/math?math=\leq x">), <img src="https://render.githubusercontent.com/render/math?math=Belady_x"> observes exact reuse distances, and sets the reuse distances of the remaining cache lines (with reuse distance <img src="https://render.githubusercontent.com/render/math?math=> x">) to <img src="https://render.githubusercontent.com/render/math?math=\infty">.
Then, <img src="https://render.githubusercontent.com/render/math?math=Belady_x"> evicts the line with the highest reuse distance, breaking ties randomly.
The cache hit rate of <img src="https://render.githubusercontent.com/render/math?math=Belady_x"> is plotted on the y-axis, normalized so that 0 and 1 correspond to the cache hit rate of LRU and <img src="https://render.githubusercontent.com/render/math?math=Belady_\infty"> (the normal unconstrained version of Belady's), respectively.
As the figure shows, a significant amount of future information is required to fully match Belady's performance.

## PARROT: Learning to Imitate Belady's

### 4.1. Model and Training Overview

**Model**.
Below, we overview the basic architecture of the PARROT policy <img src="https://render.githubusercontent.com/render/math?math=\pi_\theta(a_t | s_t)"> (Figure 3), which draws on the Transformer (Vaswani et al., 2017) and BiDAF (Seo et al., 2016) architectures.
See Appendix A for the full details.
1. Embed the current cache access <img src="https://render.githubusercontent.com/render/math?math=s^a_t = (m_t, pc_t)"> to obtain memory address embedding <img src="https://render.githubusercontent.com/render/math?math=e(m_t)"> and PC embedding <img src="https://render.githubusercontent.com/render/math?math=e(pc_t)"> and pass them through an LSTM to obtain cell state <img src="https://render.githubusercontent.com/render/math?math=c_t"> and hidden state <img src="https://render.githubusercontent.com/render/math?math=h_t">: <img src="https://render.githubusercontent.com/render/math?math=\Large c_t, h_t = LSTM([e(m_t)\ \ e(pc_t)],c_{t-1},h_{t-1})">
2. Keep the past <img src="https://render.githubusercontent.com/render/math?math=H"> hidden states, <img src="https://render.githubusercontent.com/render/math?math=[h_{t-H+\pm 1}, \dots, h_t]">, representing an embedding of the cache access history <img src="https://render.githubusercontent.com/render/math?math=s^h_t"> and current cache access <img src="https://render.githubusercontent.com/render/math?math=s^a_t">.
3. Form a context gw for each cache line <img src="https://render.githubusercontent.com/render/math?math=l_w"> in the cache state <img src="https://render.githubusercontent.com/render/math?math=s^c_t"> by embedding each line as <img src="https://render.githubusercontent.com/render/math?math=e(l_w)"> and attending over the past H hidden states with <img src="https://render.githubusercontent.com/render/math?math=e(l_w)">: <img src="https://render.githubusercontent.com/render/math?math=\Large g_w = Attention(Q,K)"> where query <img src="https://render.githubusercontent.com/render/math?math=Q = e(l_w)">, keys <img src="https://render.githubusercontent.com/render/math?math=K = [h_{t-H+\pm 1}, \dots, h_t]">
4. Apply a final dense layer and softmax on top of these line contexts to obtain the policy: <img src="https://render.githubusercontent.com/render/math?math=\pi_\theta(a_t = w | s_t) = softmax(dense(g_w))">
5. Choose <img src="https://render.githubusercontent.com/render/math?math=arg max_{a\in \mathcal{A}_{s_t}}\pi_\theta(a | s_t)"> as the replacement action to take at timestep <img src="https://render.githubusercontent.com/render/math?math=t">.

**Training**.
Algorithm 1 summarizes the training algorithm for the PARROT policy <img src="https://render.githubusercontent.com/render/math?math=\pi_\theta">.
The high-level strategy is to visit a set of states B and then update the parameters <img src="https://render.githubusercontent.com/render/math?math=\theta"> to make the same eviction decision as the optimal policy <img src="https://render.githubusercontent.com/render/math?math=\pi^*"> on each state <img src="https://render.githubusercontent.com/render/math?math=s \in B"> via the loss function <img src="https://render.githubusercontent.com/render/math?math=\mathcal{L}_\theta(s, \pi^*)">.

First, we convert a given sequence of consecutive cache accesses <img src="https://render.githubusercontent.com/render/math?math=(m_1, pc_1), \dots, (m_T, pc_T)"> into states <img src="https://render.githubusercontent.com/render/math?math=s_0, \dots, s_T"> (Section 4.2), on which we can compute the optimal action with Belady's (lines 3-5).
Given the states, we train PARROT with truncated backpropagation through time (lines 6-9).
We sample batches of consecutive states <img src="https://render.githubusercontent.com/render/math?math=s_{l-H}, \dots, s_{l+\pm H}"> and initialize the LSTM hidden state of our policy on the cache accesses of <img src="https://render.githubusercontent.com/render/math?math=s_{l-H}"> to <img src="https://render.githubusercontent.com/render/math?math=s_{l-1}">.
Then, we apply our replacement policy <img src="https://render.githubusercontent.com/render/math?math=\pi_\theta"> to each of the remaining states <img src="https://render.githubusercontent.com/render/math?math=s_l, \dots, s_{l+\pm H-1}"> in order to compute the loss <img src="https://render.githubusercontent.com/render/math?math=\mathcal{L}_\theta(s_t, \pi^*)"> (Sections 4.3 and 4.4), which encourages the learned replacement policy to make the same decisions as Belady's.

### 4.2. Avoiding Compounding Errors

Since we are only given the cache accesses and not the states, we must determine which replacement policy to follow on these cache accesses to obtain the states B. Naively, one natural policy to follow is the optimal policy .
However, this leads to compounding errors (Ross et al., 2011; Daum´e et al., 2009; Bengio et al., 2015), where the distribution of states seen during test time (when following the learned policy) differs from the distribution of states seen during training (when following the oracle policy).
At test time, since PARROT learns an imperfect approximation of the oracle policy, it will eventually make a mistake and evict a suboptimal cache line.
This leads to cache states that are different from those seen during training, which the learned policy has not trained on, leading to further mistakes.

To address this problem, we leverage the DAgger algorithm (Ross et al., 2011).
DAgger avoids compounding errors by also following the current learned policy <img src="https://render.githubusercontent.com/render/math?math=\pi_\theta"> instead of the oracle policy <img src="https://render.githubusercontent.com/render/math?math=\pi^*"> to collect <img src="https://render.githubusercontent.com/render/math?math=B"> during training, which forces the distribution of training states to match that of test states.
As PARROT updates the policy, the current policy becomes increasingly different from the policy used to collect B, causing the training state distribution <img src="https://render.githubusercontent.com/render/math?math=B"> to drift from the test state distribution.
To mitigate this, we periodically update <img src="https://render.githubusercontent.com/render/math?math=B"> every 5000 parameter updates by recollecting <img src="https://render.githubusercontent.com/render/math?math=B"> again under the current policy.
Based on the recommendation in (Ross et al., 2011), we follow the oracle policy the first time we collect <img src="https://render.githubusercontent.com/render/math?math=B">, since at that point, the policy <img src="https://render.githubusercontent.com/render/math?math=\pi_\theta"> is still random and likely to make poor eviction decisions.

Notably, this approach is possible because we can compute our oracle policy (Belady's) at any state during training, as long as the future accesses are known.
This differs from many IL tasks (Hosu & Rebedea, 2016; Vecerik et al., 2017), where querying the expert is expensive and limited.

### 4.3. Ranking Loss

Once the states <img src="https://render.githubusercontent.com/render/math?math=B"> are collected, we update our policy <img src="https://render.githubusercontent.com/render/math?math=\pi_\theta"> to better approximate Belady's <img src="https://render.githubusercontent.com/render/math?math=\pi^*"> on these states via the loss function <img src="https://render.githubusercontent.com/render/math?math=\mathcal{L}_theta(s, \pi^*)">.
A simple log-likelihood (LL) behavior cloning loss (Pomerleau, 1989) <img src="https://render.githubusercontent.com/render/math?math=\mathcal{L}(s,\pi^*) = log \pi_theta(\pi^*(s) | s)"> encourages the learned policy to place probability mass on the optimal action <img src="https://render.githubusercontent.com/render/math?math=\pi^*(s)">.
However, in the setting where the distribution <img src="https://render.githubusercontent.com/render/math?math=\pi^*(a | s)"> is known, instead of just the optimal action <img src="https://render.githubusercontent.com/render/math?math=\pi^*(s)">, optimizing to match this distribution can provide more supervision, similar to the intuition of distillation (Hinton et al., 2015).
Thus, we propose an alternate ranking loss to leverage this additional supervision.

Concretely, PARROT uses a differentiable approximation (Qin et al., 2010) of normalized discounted cumulative gain (NDCG) with reuse distance as the relevancy metric: <img src="https://render.githubusercontent.com/render/math?math=\mathcal{L}^{rank}_\theta(s_t, \pi^*) = -\frac{DCG}{IDCG}"> where <img src="https://render.githubusercontent.com/render/math?math=DCG = \sum^W_{w=1}\frac{d_t(l_w)-1}{log(pos(l_w) +\pm 1)}">, <img src="https://render.githubusercontent.com/render/math?math=pos(l_w) = \sum_{i\neq w}\sigma(-\alpha(\pi_\theta(i|s_t) - \pi_\theta(w|s_t)))">

Here, <img src="https://render.githubusercontent.com/render/math?math=pos(l_w)"> is a differentiable approximation of the rank of line <img src="https://render.githubusercontent.com/render/math?math=l_w">, ranked by how much probability the policy <img src="https://render.githubusercontent.com/render/math?math=\pi_\theta"> places on evicting <img src="https://render.githubusercontent.com/render/math?math=l_w">, where <img src="https://render.githubusercontent.com/render/math?math=\alpha = 10"> is a hyperparameter and <img src="https://render.githubusercontent.com/render/math?math=\sigma"> is the sigmoid function.
IDCG is a normalization constant set so that <img src="https://render.githubusercontent.com/render/math?math=-1 \leq \mathcal{L}^{rank}_\theta \leq 0">, equal to the value of DCG when the policy <img src="https://render.githubusercontent.com/render/math?math=\pi_\theta"> correctly places probability mass on the lines in descending order of reuse distance.
This loss function improves cache hit rates by heavily penalizing <img src="https://render.githubusercontent.com/render/math?math=\pi_\theta"> for placing probability on lines with low reuse distance, which will likely lead to cache misses, and only lightly penalizing <img src="https://render.githubusercontent.com/render/math?math=\pi_\theta"> for placing probability on lines with higher reuse distance, which are closer to being optimal and are less likely to lead to cache misses.

Optimizing our loss function is similar to optimizing the Kullback-Liebler (KL) divergence (Kullback & Leibler, 1951) between a smoothed version of Belady's, which evicts line <img src="https://render.githubusercontent.com/render/math?math=l_w"> with probability proportional to its exponentiated reuse distance <img src="https://render.githubusercontent.com/render/math?math=e^{d_t}(l_w)">, and our policy <img src="https://render.githubusercontent.com/render/math?math=\pi_\theta">.
Directly optimizing the KL between the non-smoothed oracle policy and our policy just recovers the normal LL loss, since Belady's actually places all of its probability on a single line.

### 4.4. Predicting Reuse Distance

To add further supervision during training, we propose to predict the reuse distances of each cache line as an auxiliary task (Jaderberg et al., 2016; Mirowski et al., 2016; Lample & Chaplot, 2017).
Concretely, we add a second fullyconnected head on PARROT's network that takes as inputs the per-line context embeddings <img src="https://render.githubusercontent.com/render/math?math=g_w"> and outputs predictions of the log-reuse distance <img src="https://render.githubusercontent.com/render/math?math=\^{d}(g_w)">.
We train this head with a mean-squared error loss <img src="https://render.githubusercontent.com/render/math?math=\mathcal{L}^{reuse}_\theta(s,\pi^*) = \frac{1}{W}\sum^W_{w=1}(\^{d}(g_w) - log d_t(l_w))^2">.
Intuitively, since the reuse distance predicting head shares the same body as the policy head <img src="https://render.githubusercontent.com/render/math?math=\pi_\theta">, learning to predict reuse distances helps learn better representations in the rest of the network.
Overall, we train our policy with loss <img src="https://render.githubusercontent.com/render/math?math=\mathcal{L}_\theta(s,\pi^*) = \mathcal{L}^{rank}_\theta(s,\pi^*) +\pm \mathcal{L}^{reuse}_\theta(s,\pi^*)">

### 4.5. Towards Practicality

The goal of this work is to establish directly imitating Belady's as a proof-of-concept.
Applying approaches like PARROT to real-world systems requires reducing model size and latency to prevent overshadowing improved cache replacement.
We leave these challenges to future work, but highlight one way to reduce model size in this section, and discuss further promising directions in Section 7.

In the full-sized PARROT model, we learn a separate embedding for each PC and memory address, akin to word vectors (Mikolov et al., 2013) in natural language processing.
While this approach performs well, these embeddings can require tens of megabytes to store for real-world programs that access hundreds of thousands of unique memory addresses.

To reduce model size, we propose learning a byte embedder shared across all memory addresses, only requiring several kilobytes of storage.
This byte embedder embeds each memory address (or PC) by embedding each byte separately and then passing a small linear layer over their concatenated outputs (Figure 4).
In principle, this can learn a hierarchical representation, that separately represents large memory regions (upper bytes of an address) and finer-grained objects (lower bytes).

## Experiments

### 5.1. Experimental Setup

Following Shi et al. (2019), we evaluate our approach on a three-level cache hierarchy with a 4-way 32KB L1 cache, a 8-way 256KB L2 cache, and a 16-way 2MB last-level cache.
We apply our approach to the last-level cache while using the LRU replacement policy for L1/L2 caches.

For benchmark workloads, we evaluate on the memoryintensive SPEC CPU2006 (Henning, 2006) applications used by Shi et al. (2019).
In addition, we evaluate on Google Web Search, an industrial-scale application that serves billions of queries per day, to further evaluate the effectiveness of PARROT on real-world applications with complex access patterns and large working sets.

For each of these programs, we run them and collect raw memory access traces over a 50 second interval using dynamic binary instrumentation tools (Bruening et al., 2003).
This produces the sequence of all memory accesses that the program makes during that interval.
Last-level cache access traces are obtained from this sequence by passing the raw memory accesses through the L1 and L2 caches using an LRU replacement policy.

As this produces a large amount of data, we then sample the resultant trace for our training data (Qureshi et al., 2007).
We randomly choose 64 sets and collect the accesses to those sets on the last-level cache, totaling an average of about 5M accesses per program.
Concretely, this yields a sequence of accesses <img src="https://render.githubusercontent.com/render/math?math=(m_1, pc_1), \dots, (m_T, pc_T)">.
We train replacement policies on the first 80% of this sequence, validate on the next 10%, and report test results on the final 10%.

Our evaluation focuses on two key metrics representing the efficiency of cache replacement policies.
First, as increasing cache hit rates is highly correlated to decreasing program latency (Qureshi et al., 2007; Shi et al., 2019; Jain & Lin, 2016), we evaluate our policies using raw cache hit rates.
Second, we report normalized cache hit rates, representing the gap between LRU (the most common replacement policy) and Belady's (the optimal replacement policy).
For a policy with hit rate <img src="https://render.githubusercontent.com/render/math?math=r">, we define the normalized cache hit rate as <img src="https://render.githubusercontent.com/render/math?math=\frac{r-r_{LRU}}{r_{opt} - r_{LRU}}">, where <img src="https://render.githubusercontent.com/render/math?math=r_{LRU}"> and <img src="https://render.githubusercontent.com/render/math?math=r_{opt}"> are the hit rates of LRU and Belady's, respectively.
The normalized hit rate represents the effectiveness of a given policy with respect to the two baselines, LRU (normalized hit rate of 0) and Belady's (normalized hit rate of 1).

We compare the following four approaches:
1. PARROT: trained with the full-sized model, learning a separate embedding for each PC and address.
2. PARROT (byte): trained with the much smaller byte embedder (Section 4.5).
3. Glider (Shi et al., 2019): the state-of-the-art cache replacement policy, based on the results reported in their paper.
4. Nearest Neighbor: a nearest neighbors version of Belady's, which finds the longest matching PC and memory address suffix in the training data and follows the Belady's decision of that.

The SPEC2006 program accesses we evaluate on may slightly differ from those used by Shi et al. (2019) in evaluating Glider, as the latter is not publicly available.
However, to ensure a fair comparison, we verified that the measured hit rates for LRU and Belady's on our cache accesses are close to the numbers reported by Shi et al. (2019), and we only compare on normalized cache hit rates.
Since Glider's hit rates are not available on Web Search, we compare PARROT against LRU, the policy frequently used in production CPU caches.
The reported hit rates for PARROT, LRU, Belady's, and Nearest Neighbors are measured on the test sets.
We apply early stopping on PARROT, based on the cache hit rate on the validation set.
For PARROT, we report results averaged over 3 random seeds, using the same minimallytuned hyperparameters in all domains.
These hyperparameters were tuned exclusively on the validation set of omnetpp (full details in Appendix B).

### 5.2. Main Results

Table 1 compares the raw cache hit rate of PARROT with that of Belady's and LRU.
PARROT achieves significantly higher cache hit rates than LRU on every program, ranging from 2% to 30%.
Averaged over all programs, PARROT achieves 16% higher cache hit rates than LRU.
According to prior study on cache sensitivity of SPEC2006 workloads (Jaleel, 2010), achieving the same level of cache hit rates as PARROT with LRU would require increasing the cache capacity by 2-3x (e.g., omnetpp and mcf) to 16x (e.g., libquantum).

On the Web Search benchmark, PARROT achieves a 61% higher normalized cache hit rate and 13.5% higher raw cache hit rate than LRU, demonstrating PARROT's practical ability to scale to the complex memory access patterns found in datacenter-scale workloads.

Figure 5 compares the normalized cache hit rates of PARROT and Glider.
With the full-sized model, PARROT outperforms Glider on 10 of the 13 SPEC2006 programs, achieving a 20% higher normalized cache hit rate averaged over all programs; on the remaining 3 programs (bzip, bwaves, and mcf), Glider performs marginally better.
Additionally, PARROT achieves consistent performance with low variance across seeds.

**Reducing model size**.
Though learning PARROT from scratch with the byte embedder does not perform as well as the full-sized model, the byte embedder model is significantly smaller and still achieves an average of 8% higher normalized cache hit rate than Glider (Figure 5).
In Section 7, we highlight promising future directions to reduce the performance gap and further reduce model size and latency.

**Generalization**.
An effective cache replacement policy must be able to generalize to unseen code paths (i.e., sequences of accesses) from the same program, as there are exponentially many code paths and encountering them all during training is infeasible.
We test PARROT's ability to generalize to new code paths by comparing it to the nearest neighbors baseline (Figure 5).
The performance of the nearest neighbors baseline shows that merely memorizing training code paths seen achieves near-optimal cache hit rates on simpler programs (e.g., gems, lbm), which just repeatedly execute the same code paths, but fails for more complex programs (e.g., mcf, Web Search), which exhibit highly varied code paths.
In contrast, PARROT maintains high cache hit rates even on these more complex programs, showing that it can generalize to new code paths not seen during training.

Additionally, some of the programs require generalizing to new memory addresses and program counters at test time.
In mcf, 21.6% of the test-time memory addresses did not appear in the training data, and in Web Search, 5.3% of the test-time memory addresses and 6% of the test-time PCs did not appear in the training data (full details in Appendix B), but PARROT performs well despite this.

### 5.3. Ablations

Below, we ablate each of the following from PARROT: predicting reuse distance, on-policy training (DAgger), and ranking loss.
We evaluate on four of the most memoryintensive SPEC2006 applications (lbm, libq, mcf, and omnetpp) and Web Search and compare each ablation with Glider, Belady's, and two versions of PARROT.
PARROT is the full-sized model with no ablations.
PARROT (base) is PARROT's neural architecture, with all three additions ablated.
Comparing PARROT (base) to Glider (e.g., Figure 6) shows that in some programs (e.g., omnetpp and lbm), simply casting cache replacement as an IL problem with PARROT's neural architecture is sufficient to obtain competitive performance, while in other programs, our additions are required to achieve state-of-the-art cache hit rates.

**Predicting Reuse Distance**.
Figure 6 compares the following three configurations to show the effect of incorporating reuse distance information: (i) PARROT (no reuse dist.), where reuse distance prediction is ablated, (ii) PARROT (evict highest reuse dist.), where our fully ablated model (PARROT (base)) predicts reuse distance and directly evicts the line with the highest predicted reuse distance, and (iii) PARROT (reuse dist. aux loss), where our fully ablated model learns to predict reuse distance as an auxiliary task.

Comparing PARROT (no reuse dist.) to PARROT shows that incorporating reuse distance greatly improves cache hit rates.
Between different ways to incorporate reuse distance into PARROT, using reuse distance prediction indirectly as an auxiliary loss function (PARROT (reuse dist. aux loss)) leads to higher cache hit rates than using the reuse distance predictor directly to choose which cache line to evict (PARROT (evict highest reuse dist.)).
We hypothesize that in some cache states, accurately predicting the reuse distance for each line may be challenging, but ranking the lines may be relatively easy.
Since our reuse distance predictor predicts log reuse distances, small errors may drastically affect which line is evicted when the reuse distance predictor is used directly.

**Training with DAgger**.
Figure 7 summarizes the results when ablating training on-policy with DAgger.
In theory, training off-policy on roll-outs of Belady's should lead to compounding errors, as the states visited during training under Belady's differ from those visited during test time.
Empirically, we observe that this is highly program-dependent.
In some programs, like mcf or Web Search, training offpolicy performs as well or better than training on-policy, but in other programs, training on-policy is crucial.
Overall, training on-policy leads to an average 9.8% normalized cache hit rate improvement over off-policy training.

**Ranking Loss**.
Figure 8 summarizes the results when ablating our ranking loss.
Using our ranking loss over a loglikelihood (LL) loss introduces some bias, as the true optimal policy places all its probability on the line with the highest reuse distance.
However, our ranking loss better optimizes cache hit rates, as it more heavily penalizes evicting lines with lower reuse distances, which lead to misses.
In addition, a distillation perspective of our loss, where the teacher network is an exponentially-smoothed version of Belady's with the probability of evicting a line set as proportional to exp(reuse distance), suggests that our ranking loss provides greater supervision than LL.
Tuning a temperature on the exponential smoothing of Belady's could interpolate between less bias and greater supervision.
Empirically, we observe that our ranking loss leads to an average 3.5% normalized cache hit rate improvement over LL.

### 5.4. History Length

One key question is: how much past information is needed to accurately approximate Belady's?
We study this by varying the number of past accesses that PARROT attends over (H) from 20 to 140.
In theory, PARROT's LSTM hidden state could contain information about all past accesses, but the LSTM's memory is limited in practice.

The results are summarized in Figure 9.
We observe that the past accesses become an increasingly better predictor of the future as the number past accesses increase, until about 80.
After that point, more past information doesn't appear to help approximate Belady's.
Interestingly, Shi et al. (2019) show that Glider experiences a similar saturation in improvement from additional past accesses, but at around 30 past accesses.
This suggests that learning a replacement policy end-to-end with PARROT can effectively leverage more past information than simply predicting whether a cache line is cache-friendly or cache-averse.

## Related Work

**Cache Replacement**.
Traditional approaches to cache replacement rely on heuristics built upon intuition for cache access behavior.
LRU is based on the assumption that most recently used lines are more likely to be reused.
More sophisticated policies target a handful of manually classified access patterns based on simple counters (Qureshi et al., 2007; Jaleel et al., 2010) or try to predict instructions that tend to load zero-reuse lines based on a table of saturating counters (Wu et al., 2011; Khan et al., 2010).

Several recent approaches instead focus on learning cache replacement policies.
Wang et al. (2019) also cast cache replacement as learning over a Markov decision process, but apply reinforcement learning instead of imitation learning, which results in lower performance.
More closely related to ours are Hawkeye (Jain & Lin, 2016) and Glider (Shi et al., 2019), which also learn from Belady's.
They train a binary classification model based on Belady's to predict if a line is cache-friendly or cache-averse, but rely on a traditional replacement heuristic to determine which line to evict when several lines are cache-averse.
Relying on the traditional heuristic to produce the final eviction decisions heavily constrains the expressivity of the policy class they learn over, so that even the best policy within their class of learnable policies may not accurately approximate Belady's, yielding high cache miss rates for some access patterns.

In contrast, our work is the first to propose learning a cache replacement policy end-to-end with imitation learning.
Framing cache replacement in this principled framework is important as much prior research has resorted to heuristics for hill climbing specific benchmarks.
In addition, learning end-to-end enables us to optimize over a highly expressive policy class, achieving high cache hit rates even on complex and diverse access patterns.

**Imitation Learning**.
Our work builds on imitation learning (IL) techniques (Ross & Bagnell, 2014; Sun et al., 2017), where the goal is to approximate an expert policy.
Our setting exhibits two distinctive properties: First, in our setting, the expert policy (Belady's) can be queried at any state during training.
the oracle policy (Belady's) can be cheaply queried at any state during training, which differs from a body of IL work (Vecerik et al., 2017; Hosu & Rebedea, 2016; Hester et al., 2018) focusing on learning with limited samples from an expensive expert (e.g., a human).
The ability to arbitrarily query the oracle enables us to avoid compounding errors with DAgger (Ross et al., 2011).
Second, the distribution over actions of the oracle policy is available, enabling more sophisticated loss functions.
Prior work (Sabour et al., 2018; Choudhury et al., 2017) also studies settings with these two properties, although in different domains.
Sabour et al. (2018) shows that an approximate oracle can be computed in some natural-language sequence generation tasks; Choudhury et al. (2017) learns to imitate an oracle computed from data only available during training, similar to Belady's, which requires future information.

## Conclusion and Future Directions

We develop a foundation for learning end-to-end cache replacement policies with imitation learning, which significantly bridges the gap between prior work and Belady's optimal replacement policy.
Although we evaluate our approach on CPU caches, due to the popularity of SPEC2006 as a caching benchmark, we emphasize that our approach applies to other caches as well, such as software caches, databases, and operating systems.
Software caches may be an especially promising area for applying our approach, as they tolerate higher latency in the replacement policy and implementing more complex replacement policies is easier in software.
We highlight two promising future directions:

First, this work focuses on the ML challenges of training a replacement to approximate Belady's and does not explore the practicality of deploying the learned policy in production, where the two primary concerns are the memory and latency overheads of the policy.
To address these concerns, future work could investigate model-size reduction techniques, such as distillation (Hinton et al., 2015), pruning (Janowsky, 1989; Han et al., 2015; Sze et al., 2017), and quantization, as well as domains tolerating greater latency and memory use, such as software caches.
Additionally, cache replacement decisions can be made at any time between misses to the same set, which provides a reasonably long latency window (e.g., on the order of seconds for software caches) for our replacement policy to make a decision.
Furthermore, the overall goal of cache replacement is to minimize latency.
While minimizing cache misses minimizes latency to a first approximation, cache misses incur variable amounts of latency (Qureshi et al., 2006), which could be addressed by fine-tuning learned policies to directly minimize latency via reinforcement learning.

Second, while Belady's algorithm provides an optimal replacement policy for a single-level cache, there is no known optimal policy for multiple levels of caches (as is common in CPUs and web services).
This hierarchical cache replacement policy is a ripe area for deep learning and RL research, as is exploring the connection between cache replacement and prefetching, as they both involve selecting the optimal set of lines to be present in the cache.
Cache replacement is backward looking (based on the accesses so far) while prefetching is forward looking (predicting future accesses directly (Hashemi et al., 2018; Shi et al., 2020)).

To facilitate further research in this area, we release a Gym environment for cache replacement, which easily extends to the hierarchical cache replacement setting, where RL is required as the optimal policy is unknown.
We find cache replacement an attractive problem for the RL/IL communities, as it has significant real-world impact and data is highly available, in contrast to many current benchmarks that only have one of these two properties.
In addition, cache replacement features several interesting challenges: rewards are highly delayed, as evicting a particular line may not lead to a cache hit/miss until thousands of timesteps later; the semantics of the action space dynamically changes, as the replacement policy chooses between differing cache lines at different states; the state space is large (e.g., 100,000s of unique addresses) and some programs require generalizing to new memory addresses at test time, not seen during training, similar to the rare words problem (Luong et al., 2014) in NLP; and as our ablations show, different programs exhibit wildly different cache access patterns, which can require different techniques to address.
In general, we observe that computer systems exhibit many interesting machine learning (ML) problems, but have been relatively inaccessible to the ML community because they require sophisticated systems tools.
We take steps to avoid this by releasing our cache replacement environment.

**Reproducibility**.
Code for PARROT and our cache replacement Gym environment is available at https://github.com/google-research/google-research/tree/master/cache_replacement.
