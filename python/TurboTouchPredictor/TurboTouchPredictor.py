# Flavien Volant and Géry Casiez
#
# Licence defined in LICENCE.md file
#

import math
from .OneEuroVectorProcessor import OneEuroVectorProcessor
from .TTPconfig import cfg
from .pAttribut import pAttribut

COMPENSATION_ALLOWED_VALUES = (0, 16, 32, 48, 64)

class TurboTouchPredictor:

    """ TurboTouchPredictor
    :filter: OneEuroVectorProcessor
    :latencyCompensated: int
    :update1euroInternalFreq: bool
    """

    def __init__(self) -> None:
        """ Initializes the TurboTouchPredictor
        """
        self.filter = OneEuroVectorProcessor(1, 1, 0)
        self.latencyCompensated = 0
        self.update1euroInternalFreq = False

    def __applyParametersForLatComp(self, l: int) -> None:
        """ Sets the parameters of the predictor for the given prediction duration
         
        :param l: - Prediction in ms. Allowed values: 16, 32, 48, 64
        :type l: int
        """
        self.lastState = "NotInteracting"
        self.px = pAttribut(alpha=cfg[l]["alpha_x"],
                            lambdA=cfg[l]["lambda_x"],
                            gamma=[cfg[l]["gamma1_x"], cfg[l]["gamma2_x"], cfg[l]["gamma3_x"], cfg[l]["gamma4_x"], 0, 0, 0, 0, 0, 0],
                            a=[0, 0, 0, 0, 0])
        self.py = pAttribut(alpha=cfg[l]["alpha_y"],
                            lambdA=cfg[l]["lambda_y"],
                            gamma=[cfg[l]["gamma1_y"], cfg[l]["gamma2_y"], cfg[l]["gamma3_y"], cfg[l]["gamma4_y"], 0, 0, 0, 0, 0, 0],
                            a=[0, 0, 0, 0, 0])

        # ------------------------------------------
        
        # Sc should be set to the maximum amplitude of the differentiated signal
        # Unit: that of the signal, i.e. pixels
        self.Sc = 1e4

        self.V0 = cfg[l]["V0"]
        self.V0 = self.V0 * self.V0
        self.alpha_gain = cfg[l]["alpha_gain"]
        self.Ts = cfg[l]["ts"]
        self.int_steps = 2
        self.transients_duration = cfg[l]["transients"]
        self.count_transients = 0

        # ------------------------------------------
        
        self.lx = self.ly = 0
        self.steps_counter_x = self.steps_counter_y = 0

        self.sx = [0, 0, 0, 0, 0]
        self.sy = [0, 0, 0, 0, 0]
        self.alpha = 0

        # ------------------------------------------

        self.oneEuroEnabled = cfg[l]["one_euro_enabled"]
        
        self.update1euroInternalFreq = cfg[l]["one_euro_intern_freq_update"] if "one_euro_intern_freq_update" in cfg[l] else False
        self.filter.setParameters(cfg[l]["one_euro_freq"], cfg[l]["one_euro_mincutoff"], cfg[l]["one_euro_beta"])
    

    def setAmountOfCompensation(self, comp: int) -> None:
        """ Sets the parameters of the predictor for the given amount of compensation

        :param comp: Compensation amount in ms. Allowed values: 0, 16, 32, 48, 64
        :type comp: int
        """
        
        if comp in COMPENSATION_ALLOWED_VALUES :
            self.latencyCompensated = comp
            if comp > 0:
                self.__applyParametersForLatComp(comp)
            self.reset()
        else :
            print("self value is not allowed")
        
    def predict(self, x:float, y:float, timestamp:float, state:str)  -> tuple[float, float]:
        """ Predicts a point from the current lagging one
        
        :param x: x coordinate in pixels
        :param y: y coordinate in pixels
        :param timestamp: timestamp in nanoseconds
        :param state: "Interacting" or "NotInteracting" 
        :returns: return the predicted point p, p[0]: x coordinate, p[1]: y coordinate
        :rtype: tuple[float, float]  
        """
        if self.latencyCompensated == 0:
            return x, y

        # Update HOM
        DerX = [0, 0, 0]
        DerY = [0, 0, 0]
        self.steps_counter_x = self.__updateHOM(state, self.sx, x, self.px, DerX, self.steps_counter_x)
        self.steps_counter_y = self.__updateHOM(state, self.sy, y, self.py, DerY, self.steps_counter_y)

        # Compute V

        V = math.pow(DerX[0], 2.0) + math.pow(DerY[0], 2.0)

        # Update alpha
        alpha_star = 1 if V > self.V0 else 0
        self.alpha += self.alpha_gain * (alpha_star - self.alpha)

        ts = None
        if (self.update1euroInternalFreq):
            ts = timestamp * math.pow(10, -9) # Convert to seconds for 1euro filter

        predicted = (self.__predictIntern(x, self.px, DerX, self.alpha), self.__predictIntern(y, self.py, DerY, self.alpha), ts)

        self.count_transients+=1
        res = None
        if self.count_transients <= self.transients_duration :
            res = (x, y, timestamp) # Skip first events in order to avoid noises.
        else :
            res = predicted

        if (self.oneEuroEnabled) :
            if (state != "Interacting") :
                self.filter.reset()
        
            res = self.filter.process(res, (x, y))
        
        return res

    def __D_DiffHOMD4(self, state: tuple[float, float, float, float, float], u: float, p: pAttribut) :
        e = state[0]-u 
        if (e > 0.0):
            return (-p.a[0]*math.pow(e,1+p.lambdA)+state[1],
                    -p.a[1]*math.pow(e,1+2*p.lambdA)+state[2],
                    -p.a[2]*math.pow(e,1+3*p.lambdA)+state[3],
                    -p.a[3]*math.pow(e,1+4*p.lambdA)+state[4],
                    -p.a[4]*math.pow(e,1+5*p.lambdA))
                    
        else:
            return (p.a[0]*math.pow(-e,1+p.lambdA)+state[1],
                    p.a[1]*math.pow(-e,1+2*p.lambdA)+state[2],
                    p.a[2]*math.pow(-e,1+3*p.lambdA)+state[3],
                    p.a[3]*math.pow(-e,1+4*p.lambdA)+state[4],
                    p.a[4]*math.pow(-e,1+5*p.lambdA))
                    
    

    def reset(self):
        """Resets the internal state of the processor
        """
        self.count_transients = 0
        self.filter.reset()
    

    def __updateHOM(self,
                    state: str, 
                    s: tuple[float, float, float, float, float],
                    x: float, 
                    p: pAttribut,
                    Der: list[float],
                    steps_counter: float) :
         
        doReset = (state == "Interacting" and self.lastState != "Interacting") or (state != "Interacting" and self.lastState == "Interacting") 

        if doReset :
            s[0] = x /self.Sc
            s[1] = 0.0
            s[2] = 0.0
            s[3] = 0.0
            s[4] = 0.0
            Der[0] = 0.0
            Der[1] = 0.0
            Der[2] = 0.0
            if state == "Interacting" and self.lastState != "Interacting":
                steps_counter = self.transients_duration
        else :
            dt = self.Ts / self.int_steps
            for _ in range(self.int_steps) :
                diffhom = self.__D_DiffHOMD4(s, x/self.Sc, p)
                s[0] = s[0] + diffhom[0] * dt
                s[1] = s[1] + diffhom[1] * dt
                s[2] = s[2] + diffhom[2] * dt
                s[3] = s[3] + diffhom[3] * dt
                s[4] = s[4] + diffhom[4] * dt
        
            if (steps_counter > 0):
                steps_counter-=1
        
        
        transient_scale = max(1.0-steps_counter/(self.transients_duration/4.0), 0.0)
        Der[0] = s[1] * self.Sc * transient_scale
        Der[1] = s[2] * self.Sc * transient_scale
        Der[2] = s[3] * self.Sc * transient_scale

        self.lastState = state

        return steps_counter
    

    def __predictIntern(self,
                        x: float,
                        p: pAttribut,
                        Der: list[float], 
                        alpha: float) :
        
        Delta = 0
        Delta += Der[0] * p.gamma[1]
        Delta += Der[1] * p.gamma[2]
        Delta += Der[2] * p.gamma[3]
        Delta += math.pow(Der[0],2.0) * p.gamma[4]
        Delta += math.pow(Der[1],2.0) * p.gamma[5]
        Delta += math.pow(Der[2],2.0) * p.gamma[6]
        Delta += abs(Der[0]) * p.gamma[7]
        Delta += abs(Der[1]) * p.gamma[8]
        Delta += abs(Der[2]) * p.gamma[9]

        return x + alpha*Delta
    
