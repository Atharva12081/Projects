import numpy as np

class Target:
    def __init__(self, position, velocity):
        self.position = np.array(position, dtype=float)
        self.velocity = np.array(velocity, dtype=float)

    def update(self, dt):
        self.position += self.velocity * dt
