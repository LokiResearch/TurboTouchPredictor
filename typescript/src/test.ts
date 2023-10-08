import { TurboTouchPredictor } from './TurboTouchPredictor'

let ttpPredictor = new TurboTouchPredictor();

// Amount of prediction in ms. Allowed values: 0, 16, 32, 48, 64
ttpPredictor.setAmountOfCompensation(32);

let predictedPoint = ttpPredictor.predict({x: 0, y: 0, t: 0, state: "Interacting"});

console.log(predictedPoint);
