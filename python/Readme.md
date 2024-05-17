[![NPM Version](https://badge.fury.io/js/turbotouchpredictor.svg?style=flat)](https://npmjs.org/package/turbotouchpredictor)
[![npm downloads](https://img.shields.io/npm/dm/turbotouchpredictor.svg?style=flat-square)](https://npm-stat.com/charts.html?package=turbotouchpredictor)

# TurboTouch predictor Python version

Provides a Python implementation for the [TurboTouch predictor](https://ns.inria.fr/loki/TTp/).

## Install

```TODO```

## Minimal example

```
from TurboTouchPredictor import TurboTouchPredictor

ttpPredictor = TurboTouchPredictor();

// Amount of prediction in ms. Allowed values: 0, 16, 32, 48, 64
ttpPredictor.setAmountOfCompensation(32);

predictedPoint = ttpPredictor.predict(0, 0, 0, "Interacting") # x, y, t, state
```

## Doc

<section id="turbotouchpredictor-package">

# TurboTouchPredictor package

<dl class="py class">

<dt class="sig sig-object py" id="TurboTouchPredictor.TurboTouchPredictor">_<span class="pre">class</span> <span class="w"></span> _<span class="sig-prename descclassname"><span class="pre">TurboTouchPredictor.</span></span><span class="sig-name descname"><span class="pre">TurboTouchPredictor</span>

<dd>

Bases: `object`

<dl class="field-list simple">

<dt class="field-odd">Filter<span class="colon">:</span></dt>

<dd class="field-odd">

OneEuroVectorProcessor

</dd>

<dt class="field-even">LatencyCompensated<span class="colon">:</span></dt>

<dd class="field-even">

int

</dd>

<dt class="field-odd">Update1euroInternalFreq<span class="colon">:</span></dt>

<dd class="field-odd">

bool

</dd>

</dl>

<dl class="py method">

<dt class="sig sig-object py" id="TurboTouchPredictor.TurboTouchPredictor.__init__"><span class="sig-name descname"><span class="pre">__init__</span></span><span class="sig-paren">(</span><span class="sig-paren">)</span> <span class="sig-return"><span class="sig-return-icon">→</span> <span class="sig-return-typehint"><span class="pre">None</span></span></span></dt>

<dd>

Initializes the TurboTouchPredictor

</dd>

</dl>

<dl class="py method">

<dt class="sig sig-object py" id="TurboTouchPredictor.TurboTouchPredictor.predict"><span class="sig-name descname"><span class="pre">predict</span></span><span class="sig-paren">(</span>_<span class="n"><span class="pre">x</span></span><span class="p"><span class="pre">:</span></span> <span class="w"></span> <span class="n"><span class="pre">float</span></span>_, _<span class="n"><span class="pre">y</span></span><span class="p"><span class="pre">:</span></span> <span class="w"></span> <span class="n"><span class="pre">float</span></span>_, _<span class="n"><span class="pre">timestamp</span></span><span class="p"><span class="pre">:</span></span> <span class="w"></span> <span class="n"><span class="pre">float</span></span>_, _<span class="n"><span class="pre">state</span></span><span class="p"><span class="pre">:</span></span> <span class="w"></span> <span class="n"><span class="pre">str</span></span>_<span class="sig-paren">)</span> <span class="sig-return"><span class="sig-return-icon">→</span> <span class="sig-return-typehint"><span class="pre">tuple</span><span class="p"><span class="pre">[</span></span><span class="pre">float</span><span class="p"><span class="pre">,</span></span> <span class="w"></span> <span class="pre">float</span><span class="p"><span class="pre">]</span></span></span></span></dt>

<dd>

Predicts a point from the current lagging one

<dl class="field-list simple">

<dt class="field-odd">Parameters<span class="colon">:</span></dt>

<dd class="field-odd">

*   **x** – x coordinate in pixels

*   **y** – y coordinate in pixels

*   **timestamp** – timestamp in nanoseconds

*   **state** – “Interacting” or “NotInteracting”

</dd>

<dt class="field-even">Returns<span class="colon">:</span></dt>

<dd class="field-even">

return the predicted point p, p[0]: x coordinate, p[1]: y coordinate

</dd>

<dt class="field-odd">Return type<span class="colon">:</span></dt>

<dd class="field-odd">

tuple[float, float]

</dd>

</dl>

</dd>

</dl>

<dl class="py method">

<dt class="sig sig-object py" id="TurboTouchPredictor.TurboTouchPredictor.reset"><span class="sig-name descname"><span class="pre">reset</span></span><span class="sig-paren">(</span><span class="sig-paren">)</span></dt>

<dd>

Resets the internal state of the processor

</dd>

</dl>

<dl class="py method">

<dt class="sig sig-object py" id="TurboTouchPredictor.TurboTouchPredictor.setAmountOfCompensation"><span class="sig-name descname"><span class="pre">setAmountOfCompensation</span></span><span class="sig-paren">(</span>_<span class="n"><span class="pre">comp</span></span><span class="p"><span class="pre">:</span></span> <span class="w"></span> <span class="n"><span class="pre">int</span></span>_<span class="sig-paren">)</span> <span class="sig-return"><span class="sig-return-icon">→</span> <span class="sig-return-typehint"><span class="pre">None</span></span></span></dt>

<dd>

Sets the parameters of the predictor for the given amount of compensation

<dl class="field-list simple">

<dt class="field-odd">Parameters<span class="colon">:</span></dt>

<dd class="field-odd">

**comp** (_int_) – Compensation amount in ms. Allowed values: 0, 16, 32, 48, 64

</dd>

</dl>

</dd>

</dl>

</dd>

</dl>





<dl class="py class" id="module-OneEuroVectorProcessor">

<dt class="sig sig-object py" id="OneEuroVectorProcessor.OneEuroVectorProcessor">_<span class="pre">class</span> <span class="w"></span> _<span class="sig-prename descclassname"><span class="pre">OneEuroVectorProcessor.</span></span><span class="sig-name descname"><span class="pre">OneEuroVectorProcessor</span></span><span class="sig-paren">(</span>_<span class="n"><span class="pre">freq</span></span><span class="p"><span class="pre">:</span></span> <span class="w"></span> <span class="n"><span class="pre">float</span></span>_, _<span class="n"><span class="pre">mincutoff</span></span><span class="p"><span class="pre">:</span></span> <span class="w"></span> <span class="n"><span class="pre">float</span></span>_, _<span class="n"><span class="pre">beta</span></span><span class="p"><span class="pre">:</span></span> <span class="w"></span> <span class="n"><span class="pre">float</span></span>_<span class="sig-paren">)</span>>

<dd>

Bases: `object`

<dl class="field-list simple">

<dt class="field-odd">NormFilter<span class="colon">:</span></dt>

<dd class="field-odd">

OneEuroFilter

</dd>

<dt class="field-even">FilterX<span class="colon">:</span></dt>

<dd class="field-even">

OneEuroFilter

</dd>

<dt class="field-odd">FilterY<span class="colon">:</span></dt>

<dd class="field-odd">

OneEuroFilter

</dd>

</dl>

<dl class="py method">

<dt class="sig sig-object py" id="OneEuroVectorProcessor.OneEuroVectorProcessor.__init__"><span class="sig-name descname"><span class="pre">__init__</span></span><span class="sig-paren">(</span>_<span class="n"><span class="pre">freq</span></span><span class="p"><span class="pre">:</span></span> <span class="w"></span> <span class="n"><span class="pre">float</span></span>, <span class="n"><span class="pre">mincutoff</span></span><span class="p"><span class="pre">:</span></span> <span class="w"></span> <span class="n"><span class="pre">float</span></span>, <span class="n"><span class="pre">beta</span></span><span class="p"><span class="pre">:</span></span> <span class="w"></span> <span class="n"><span class="pre">float</span></span><span class="sig-paren">)</span> <span class="sig-return"><span class="sig-return-icon">→</span> <span class="sig-return-typehint"><span class="pre">None</span></span></span></dt>

<dd>

Initializes the OneEuroVectorProcessor with three OneEuroFilter

<dl class="field-list simple">

<dt class="field-odd">Parameters<span class="colon">:</span></dt>

<dd class="field-odd">

*   **freq** (float) – An estimate of the frequency in Hz of the signal (> 0).

*   **mincutoff** (float) – Min cutoff frequency in Hz (> 0).

*   **beta** (float) – Parameter to reduce latency (> 0).

</dd>

</dl>

</dd>

</dl>

<dl class="py method">

<dt class="sig sig-object py" id="OneEuroVectorProcessor.OneEuroVectorProcessor.process"><span class="sig-name descname"><span class="pre">process</span></span><span class="sig-paren">(</span><span class="n"><span class="pre">prediction</span></span><span class="p"><span class="pre">:</span></span> <span class="w"></span> <span class="n"><span class="pre">tuple</span><span class="p"><span class="pre">[</span></span><span class="pre">float</span><span class="p"><span class="pre">,</span></span> <span class="w"></span> <span class="pre">float</span><span class="p"><span class="pre">,</span></span> <span class="w"></span> <span class="pre">float</span><span class="p"><span class="pre">]</span></span></span>, <span class="n"><span class="pre">lag</span></span><span class="p"><span class="pre">:</span></span> <span class="w"></span> <span class="n"><span class="pre">tuple</span><span class="p"><span class="pre">[</span></span><span class="pre">float</span><span class="p"><span class="pre">,</span></span> <span class="w"></span> <span class="pre">float</span><span class="p"><span class="pre">]</span></span></span><span class="sig-paren">)</span> <span class="sig-return"><span class="sig-return-icon">→</span> <span class="sig-return-typehint"><span class="pre">tuple</span><span class="p"><span class="pre">[</span></span><span class="pre">float</span><span class="p"><span class="pre">,</span></span> <span class="w"></span> <span class="pre">float</span><span class="p"><span class="pre">]</span></span></span></span></dt>

<dd>

<dl class="field-list simple">

<dt class="field-odd">Parameters<span class="colon">:</span></dt>

<dd class="field-odd">

*   **prediction** (tuple[float, float, float]) – (x, y, t)

*   **lag** (tuple[float, float]) – (x, y)

</dd>

<dt class="field-even">Returns<span class="colon">:</span></dt>

<dd class="field-even">

the processed position

</dd>

<dt class="field-odd">Return type<span class="colon">:</span></dt>

<dd class="field-odd">

tuple[float, float]

</dd>

</dl>

</dd>

</dl>

<dl class="py method">

<dt class="sig sig-object py" id="OneEuroVectorProcessor.OneEuroVectorProcessor.reset"><span class="sig-name descname"><span class="pre">reset</span></span><span class="sig-paren">(</span><span class="sig-paren">)</span> <span class="sig-return"><span class="sig-return-icon">→</span> <span class="sig-return-typehint"><span class="pre">None</span></span></span></dt>

<dd>

Resets the internal state of the processor

</dd>

</dl>

<dl class="py method">

<dt class="sig sig-object py" id="OneEuroVectorProcessor.OneEuroVectorProcessor.setParameters"><span class="sig-name descname"><span class="pre">setParameters</span></span><span class="sig-paren">(</span><span class="n"><span class="pre">freq</span></span><span class="p"><span class="pre">:</span></span> <span class="w"></span> <span class="n"><span class="pre">float</span></span>, <span class="n"><span class="pre">mincutoff</span></span><span class="p"><span class="pre">:</span></span> <span class="w"></span> <span class="n"><span class="pre">float</span></span>, <span class="n"><span class="pre">beta</span></span><span class="p"><span class="pre">:</span></span> <span class="w"></span> <span class="n"><span class="pre">float</span></span><span class="sig-paren">)</span> <span class="sig-return"><span class="sig-return-icon">→</span> <span class="sig-return-typehint"><span class="pre">None</span></span></span></dt>

<dd>

<dl class="field-list simple">

<dt class="field-odd">Parameters<span class="colon">:</span></dt>

<dd class="field-odd">

*   **freq** (float) – An estimate of the frequency in Hz of the signal (> 0).

*   **mincutoff** (float) – Min cutoff frequency in Hz (> 0).

*   **beta** (float) – Parameter to reduce latency (> 0).

</dd>

</dl>

</dd>

</dl>

</dd>

</dl>

</section>

</section>

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

