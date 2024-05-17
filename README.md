[![Tests](https://github.com/LokiResearch/TurboTouchPredictor/actions/workflows/tests.yml/badge.svg)](https://github.com/LokiResearch/TurboTouchPredictor/actions/workflows/tests.yml)
[![NPM Version](https://badge.fury.io/js/turbotouchpredictor.svg?style=flat)](https://npmjs.org/package/turbotouchpredictor) [![NPM Version](https://img.shields.io/pypi/v/TurboTouchPredictor)](https://pypi.org/project/TurboTouchPredictor/)

# TurboTouch predictor

Provides implementations for the [TurboTouch predictor](https://ns.inria.fr/loki/TTp/).

End-to-end latency in interactive systems is detrimental to performance and usability, and comes from a combination of hardware and software delays. TurboTouch predictor is prediction algorithm originally designed for direct touch devices based on (i) a state-of-the-art finite-time derivative estimator, (ii) a smoothing mechanism based on input speed, and (iii) a post-filtering of the prediction in two steps. Using both a pre-existing dataset of touch input as benchmark, and subjective data from a new user study, we show in the paper that this new predictor outperforms the predictors currently available in the literature and industry, based on metrics that model user-defined negative side-effects caused by input prediction. In particular, we show that our predictor can predict up to 2 or 3 times further than existing techniques with minimal negative side-effects.

## Want to contribute?

1. [Fork](https://github.com/LokiResearch/TurboTouchPredictor/fork) the repo.
1. Create a folder with your implementation and add the files for your implementation.
1. Create a file that can be executed to output in a console the result of the filtering for the ground truth. See the existing files as examples. Create a Makefile to test your implementation (see the other examples).
1. Update [docker/Dokerfile](docker/Dokerfile) to install what could be missing to compile your code.
1. Update [Makefile](Makefile) to call your makefile.
1. Create a pool request.

## Ground truth data
[strokesPredicted64GroundTruth.csv](strokesPredicted64GroundTruth.csv) has been generated using [this version of the C++ implementation](https://github.com/LokiResearch/TurboTouchPredictor/blob/c02bacfb53d6ff50412f106a948c2c07d6a1feb5/cpp/lag/predictor/TurbotouchPredictor.cpp), with the settings for 64 ms prediction. ```strokesPredicted64GroundTruth.csv``` is used to check other implementations.

# Optimization

```optimization``` folder contains scripts and additional instructions for optimizing the TurboTouch predictor for a given latency, using strokes recorded for a given device.

# InteractiveDemo

[Check the interactive demo](https://github.com/LokiResearch/TurboTouchPredictorInteractiveDemo).


## Related publication

[![DOI](https://img.shields.io/badge/doi-10.1145%2F3242587.3242646-blue)](https://doi.org/10.1145/3242587.3242646)

```
@inproceedings{10.1145/3242587.3242646,
    author = {Nancel, Mathieu and Aranovskiy, Stanislav and Ushirobira, Rosane and Efimov, Denis and Poulmane, Sebastien and Roussel, Nicolas and Casiez, G\'{e}ry},
    title = {Next-Point Prediction for Direct Touch Using Finite-Time Derivative Estimation},
    year = {2018},
    isbn = {9781450359481},
    publisher = {Association for Computing Machinery},
    address = {New York, NY, USA},
    url = {https://doi.org/10.1145/3242587.3242646},
    doi = {10.1145/3242587.3242646},
    abstract = {End-to-end latency in interactive systems is detrimental to performance and usability, and comes from a combination of hardware and software delays. While these delays are steadily addressed by hardware and software improvements, it is at a decelerating pace. In parallel, short-term input prediction has shown promising results in recent years, in both research and industry, as an addition to these efforts. We describe a new prediction algorithm for direct touch devices based on (i) a state-of-the-art finite-time derivative estimator, (ii) a smoothing mechanism based on input speed, and (iii) a post-filtering of the prediction in two steps. Using both a pre-existing dataset of touch input as benchmark, and subjective data from a new user study, we show that this new predictor outperforms the predictors currently available in the literature and industry, based on metrics that model user-defined negative side-effects caused by input prediction. In particular, we show that our predictor can predict up to 2 or 3 times further than existing techniques with minimal negative side-effects.},
    booktitle = {Proceedings of the 31st Annual ACM Symposium on User Interface Software and Technology},
    pages = {793–807},
    numpages = {15},
    keywords = {touch input, latency, lag, prediction technique},
    location = {Berlin, Germany},
    series = {UIST '18}
}
```


