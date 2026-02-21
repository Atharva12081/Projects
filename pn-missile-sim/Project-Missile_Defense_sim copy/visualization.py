import matplotlib.pyplot as plt

def plot_trajectories(target_history, interceptor_history):
    # Unzip trajectory lists to separate x and y coordinates
    target_history = list(zip(*target_history))
    interceptor_history = list(zip(*interceptor_history))
    
    plt.figure(figsize=(8,6))
    plt.plot(target_history[0], target_history[1], label="Target")
    plt.plot(interceptor_history[0], interceptor_history[1], label="Interceptor")
    
    plt.xlabel("X Position (m)")
    plt.ylabel("Y Position (m)")
    plt.legend()
    plt.title("Missile Interception Simulation")
    plt.grid()
    plt.show()

def plot_distance(time_history, distance_history):
    plt.figure()
    plt.plot(time_history, distance_history)
    plt.xlabel("Time (s)")
    plt.ylabel("Distance (m)")
    plt.title("Miss Distance vs Time")
    plt.grid()
    plt.show()
