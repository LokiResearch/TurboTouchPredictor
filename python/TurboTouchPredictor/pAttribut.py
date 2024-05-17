import math


class pAttribut :
    def __init__(self, alpha, lambdA, gamma, a):
        self.alpha = alpha
        self.lambdA = lambdA
        self.gamma = gamma
        self.a = a

        self.a[0] = 5*self.alpha
        self.a[1] = 10*math.pow(self.alpha,2.0)
        self.a[2] = 10*math.pow(self.alpha,3.0)
        self.a[3] = 5*math.pow(self.alpha,4.0)
        self.a[4] = math.pow(self.alpha,5.0)