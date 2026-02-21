import numpy as np
from guidance import proportional_navigation

MAX_ACCEL = 200  # Physical constraint on missile maneuverability

class Interceptor:
    def __init__(self, position, speed, target_initial_pos):
        self.position = np.array(position, dtype=float)
        self.speed = speed
        
        # Launch towards initial target position (realistic missile behavior)
        direction = target_initial_pos - self.position
        direction = direction / np.linalg.norm(direction)
        self.velocity = direction * speed

    def update(self, dt, target_pos, target_vel):
        # Compute proportional navigation guidance acceleration
        acc = proportional_navigation(
            self.position,
            self.velocity,
            target_pos,
            target_vel
        )
        
        # Limit acceleration magnitude to realistic physical constraints
        acc_mag = np.linalg.norm(acc)
        if acc_mag > MAX_ACCEL:
            acc = acc / acc_mag * MAX_ACCEL
        
        self.velocity += acc * dt
        
        # Maintain constant speed (ramjet/rocket constraint)
        self.velocity = self.velocity / np.linalg.norm(self.velocity) * self.speed
        
        self.position += self.velocity * dt
