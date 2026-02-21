import numpy as np
from config import RADAR_NOISE_STD

class Radar:
    def measure(self, true_position):
        # Add Gaussian noise to simulate imperfect measurements
        noise = np.random.normal(0, RADAR_NOISE_STD, size=2)
        return true_position + noise
