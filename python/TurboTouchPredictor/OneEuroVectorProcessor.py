import math
from OneEuroFilter import OneEuroFilter

class OneEuroVectorProcessor:

    """ OneEuroVectorProcessor
    :normFilter: OneEuroFilter
    :filterX: OneEuroFilter
    :filterY: OneEuroFilter
    """

    def __init__(self, freq : float, mincutoff : float, beta : float) -> None:
        """ Initializes the OneEuroVectorProcessor with three OneEuroFilter

        :param freq: An estimate of the frequency in Hz of the signal (> 0).
        :type freq: float
        :param mincutoff: Min cutoff frequency in Hz (> 0).
        :type mincutoff: float
        :param  beta: Parameter to reduce latency (> 0).
        :type beta: float
        """

        self.normFilter = OneEuroFilter(freq, mincutoff, beta, 1)
        self.filterX = OneEuroFilter(freq, mincutoff, beta, 1)
        self.filterY = OneEuroFilter(freq, mincutoff, beta, 1)
    
    def setParameters(self, freq : float, mincutoff : float, beta : float) -> None:
        """
        :param freq: An estimate of the frequency in Hz of the signal (> 0).
        :type freq: float
        :param mincutoff: Min cutoff frequency in Hz (> 0).
        :type mincutoff: float
        :param  beta: Parameter to reduce latency (> 0).
        :type beta: float
        """
        self.normFilter.setParameters(freq, mincutoff, beta)
        self.filterX.setParameters(freq, mincutoff, beta)
        self.filterY.setParameters(freq, mincutoff, beta)
        self.reset()
    

    def reset(self) -> None:
        """Resets the internal state of the processor
        """
        self.normFilter.reset()
        self.filterX.reset()
        self.filterY.reset()


    def process(self, prediction: tuple[float, float, float], lag: tuple[float, float]) -> tuple[float, float]:
        """
        :param prediction: (x, y, t)
        :type prediction: tuple[float, float, float]
        :param lag: (x, y)
        :type lag: tuple[float, float]
        :returns: the processed position
        :rtype: tuple[float, float]
        """
        dX = prediction[0] - lag[0] 
        dY = prediction[1] - lag[1] 
        norm = math.sqrt(dX*dX + dY*dY)

        if norm == 0:
            return prediction

        filteredNorm = self.normFilter.filter(norm, prediction[2])

        filteredEventX = lag[0] + dX / norm * filteredNorm
        filteredEventY = lag[1] + dY / norm * filteredNorm

        return self.filterX.filter(filteredEventX, prediction[2]), self.filterY.filter(filteredEventY, prediction[2])
    