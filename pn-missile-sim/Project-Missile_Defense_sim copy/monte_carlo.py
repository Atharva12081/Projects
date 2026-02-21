import numpy as np
from config import *
from target import Target
from interceptor import Interceptor
from radar import Radar

def single_run():
    target = Target(TARGET_INIT_POS, TARGET_VEL)
    interceptor = Interceptor(INTERCEPTOR_INIT_POS, INTERCEPTOR_SPEED, TARGET_INIT_POS)
    radar = Radar()
    
    time = 0
    
    while time < SIM_TIME:
        target.update(DT)
        measured_target_pos = radar.measure(target.position)
        interceptor.update(DT, measured_target_pos, target.velocity)
        
        distance = np.linalg.norm(target.position - interceptor.position)
        if distance < HIT_RADIUS:
            return True
        
        time += DT
    
    return False


def run_monte_carlo(n_runs=100):
    success = 0
    
    for _ in range(n_runs):
        if single_run():
            success += 1
    
    success_rate = success / n_runs * 100
    print(f"Monte Carlo Runs: {n_runs}")
    print(f"Successful Interceptions: {success}")
    print(f"Success Rate: {success_rate:.2f}%")
