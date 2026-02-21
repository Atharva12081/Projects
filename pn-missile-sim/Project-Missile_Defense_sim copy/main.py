import numpy as np
from config import *
from target import Target
from interceptor import Interceptor
from radar import Radar
from visualization import plot_trajectories, plot_distance
from monte_carlo import run_monte_carlo

def run_simulation():
    target = Target(TARGET_INIT_POS, TARGET_VEL)
    interceptor = Interceptor(INTERCEPTOR_INIT_POS, INTERCEPTOR_SPEED, TARGET_INIT_POS)
    radar = Radar()
    
    target_history = []
    interceptor_history = []
    distance_history = []
    time_history = []
    
    time = 0
    
    # Simulation loop: update physics and check for interception
    while time < SIM_TIME:
        target.update(DT)
        
        # Radar provides noisy position measurement
        measured_target_pos = radar.measure(target.position)
        
        interceptor.update(DT, measured_target_pos, target.velocity)
        
        target_history.append(target.position.copy())
        interceptor_history.append(interceptor.position.copy())
        
        # Check if interceptor is within hit radius of target
        distance = np.linalg.norm(target.position - interceptor.position)
        distance_history.append(distance)
        time_history.append(time)
        
        if distance < HIT_RADIUS:
            print(f"Target intercepted at time {time:.2f} seconds!")
            break
        
        time += DT
    
    print(f"Final miss distance: {distance:.2f} m")
    
    plot_trajectories(target_history, interceptor_history)
    plot_distance(time_history, distance_history)

if __name__ == "__main__":
    run_simulation()
    
    print("\nRunning Monte Carlo Analysis...")
    run_monte_carlo(100)
