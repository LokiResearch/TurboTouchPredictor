[![NPM Version](https://badge.fury.io/js/turbotouchpredictor.svg?style=flat)](https://npmjs.org/package/turbotouchpredictor)
[![npm downloads](https://img.shields.io/npm/dm/turbotouchpredictor.svg?style=flat-square)](https://npm-stat.com/charts.html?package=turbotouchpredictor)

# TurboTouch predictor typescript version

Provides a typescript implementation for the [TurboTouch predictor](https://ns.inria.fr/loki/TTp/).

## Install

```npm install turbotouchpredictor```

## Minimal example

```
import { TurboTouchPredictor } from 'TurboTouchPredictor'

let ttpPredictor = new TurboTouchPredictor();

// Amount of prediction in ms. Allowed values: 0, 16, 32, 48, 64
ttpPredictor.setAmountOfCompensation(32);

let predictedPoint = ttpPredictor.predict({x: 0, y: 0, t: 0, state: "Interacting"});

```

## Doc

### constructor

• **new TurboTouchPredictor**()

### predict

▸ **predict**(`e`): `any`

Predicts a point from the current lagging one

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `e` | `Object` | Lagging event |
| `e.state` | `string` | "Interacting" or "NotInteracting" |
| `e.t` | `number` | timestamp in nanoseconds |
| `e.x` | `number` | x coordinate in pixels |
| `e.y` | `number` | y coordinate in pixels |

#### Returns

`any`

- predicted point p, p.x: x corrdinate, p.y: t corrdinate, p.t: timestamp in nanoseconds

### reset

▸ **reset**(): `void`

### setAmountOfCompensation

▸ **setAmountOfCompensation**(`comp`): `void`

Sets the parameters of the predictor for the given amount of compensation

#### Parameters

| Name | Type | Description |
| :------ | :------ | :------ |
| `comp` | `number` | Compensation amount in ms. Allowed values: 0, 16, 32, 48, 64 |

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

