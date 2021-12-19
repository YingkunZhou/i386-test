# Learning Forward Reuse Distance

https://arxiv.org/pdf/2007.15859.pdf

## Abstract

Caching techniques are widely used in the era of
cloud computing from applications, such as Web caches to
infrastructures, Memcached and memory caches in computer
architectures. Prediction of cached data can greatly help improve
cache management and performance. The recent advancement of
deep learning techniques enables the design of novel intelligent
cache replacement policies.
In this work, we propose a learning-aided approach to predict
future data accesses. We find that a powerful LSTM-based
recurrent neural network model can provide high prediction
accuracy based on only a cache trace as input. The high accuracy
results from a carefully crafted locality-driven feature design.
Inspired by the high prediction accuracy, we propose a pseudo
OPT policy and evaluate it upon 13 real-world storage workloads
from Microsoft Research. Results demonstrate that the new cache
policy improves the state-of-art practical policies by up to 19.2%
and incurs only 2.3% higher miss ratio than OPT on average.
Index Terms—Deep learning, cache, performance measure-
ment, performane prediction, forward reuse distance.
