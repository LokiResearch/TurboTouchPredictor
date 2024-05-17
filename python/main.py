# Flavien Volant and Géry Casiez

import sys
from os import path
sys.path.append(f'{path.dirname(__file__)}/TurboTouchPredictor')

from TurboTouchPredictor.TurboTouchPredictor import TurboTouchPredictor


if __name__ == "__main__":

    ttp = TurboTouchPredictor()
    ttp.setAmountOfCompensation(64)

    data = open("../strokes.csv", "r")

    data.readline() # When X Y Type
    print("When,X,Y,Typestate,predX,predY")

    i = 0

    for line in data.readlines():
        t, x, y, s = tuple(line.split("\n")[0].split(","))
        state = "Interacting"
        if s in ["DOWN", "UP"]:
            state = "NotInteracting"
        
        predictedPoint = ttp.predict(float(x), float(y), float(t), state)
        print(t, x, y, s, predictedPoint[0], predictedPoint[1], sep=',')
            
        i+=1

