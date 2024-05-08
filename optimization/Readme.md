# Tuning TurboTouch predictor

## Requirements

* Python 3.x
* Matlab 

## Data

```OptitrackData``` contains data used for the optimization of TurboTouch predictor in the paper and corresponding to files available in ```cpp/config```. You may not find the exact same values depending on your version of Matlab.

## Process
```optim_predictor.py $input $fs $l  -s -v -m```

for example ```./optim_predictor.py OptitrackData 60 64 -v -m```

* $input: folder with csv file
* $fs: frequency of input
* $l: amount of latency to compensate
* -s: consider axis are symmetric. Optimization for x will used for y
* -v: Verbose

output is an 'optparam.cfg' file with optimized parameters.

