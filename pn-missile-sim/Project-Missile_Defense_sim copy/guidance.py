import numpy as np
from config import NAVIGATION_CONSTANT

def proportional_navigation(interceptor_pos, interceptor_vel,
                            target_pos, target_vel):
    """
    Proportional Navigation Guidance (PNG):
    Acceleration is proportional to closing velocity × line-of-sight rate
    This ensures the interceptor "leads" the target effectively.
    """
    relative_pos = target_pos - interceptor_pos
    relative_vel = target_vel - interceptor_vel
    
    distance = np.linalg.norm(relative_pos)
    
    if distance < 1e-3:
        return np.array([0.0, 0.0])
    
    # Line-of-sight rate: how fast the target is moving across our view
    lambda_dot = (relative_pos[0]*relative_vel[1] - 
                  relative_pos[1]*relative_vel[0]) / (distance**2 + 1e-6)
    
    # Closing velocity: how fast target approaches (positive = getting closer)
    closing_velocity = -np.dot(relative_pos, relative_vel) / distance
    
    if closing_velocity <= 0:
        return np.array([0.0, 0.0])
    
    # Core PNG equation: a = N × V_c × λ_dot
    acceleration_magnitude = NAVIGATION_CONSTANT * closing_velocity * lambda_dot
    
    # Perpendicular direction to apply acceleration (perpendicular to line-of-sight)
    perp = np.array([-relative_pos[1], relative_pos[0]]) / distance
    
    acceleration = acceleration_magnitude * perp
    
    return acceleration
