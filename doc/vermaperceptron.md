# Perceptron Learning Driven Coherence Aware Reuse Prediction for Last-level Caches

https://snehilverma41.github.io/TAMU_Report.pdf

## Abstract

Inclusive caches have been widely used in Chip
Multiprocessors in favor of the simplicity of cache coherence
protocol. However, it trades the coherence simplicity for effective
cache capacity unlike the non-inclusive caches. Cache blocks
that get evicted from the last-level cache (LLC) need to be
invalidated in higher levels of cache in order to maintain the
inclusion property. It is possible that highly referenced blocks
in the higher levels of cache (L1 and L2) can get selected as
victims in the LLC, which may incur more coherence traffic
and limit system performance. Thus, it is important to predict
highly reused blocks in the whole cache hierarchy and incorpo-
rate the prediction with cache management policies for better
performance. We propose Coherence-Aware Reuse Prediction
(CARP) to use cache coherence information of an application
as a feature in perceptron learning based reuse prediction. We
use the same for better cache management at the LLC. We
observe that CARP provides marginal performance improvement
for most of the parallel applications from PARSEC benchmark
suite. Additionally, we show the effectiveness of CARP by running
multiple parallel applications concurrently.
