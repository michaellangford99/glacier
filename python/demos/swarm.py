import dearpygui.dearpygui as dpg
import random
import time

import numpy as np


N = 1000
global position
global velocity
position = np.random.uniform(-10, 10, (N,2))
velocity = np.random.uniform(-1, 1, (N,2))

def update_flock(pos:np.ndarray, vel):
    position = pos
    velocity = vel

    heading = velocity / np.linalg.norm(velocity, axis=1, keepdims=True)
    speed = np.linalg.norm(velocity, axis=1, keepdims=True)

    center_of_mass = position.mean(axis=0)
    target_heading = center_of_mass-position
    target_heading = target_heading / np.linalg.norm(target_heading, axis=1, keepdims=True)

    coherence = 0.001

    heading = (1-coherence)*heading + coherence*target_heading
    heading = heading / np.linalg.norm(heading, axis=1, keepdims=True)

    new_vel = heading*speed
    
    for i in range(N):
        
        #for each element
        # find distance to all others
        # for others within (X) dist
        # steer away from each one weighted by inverse distance
    
        # for others within (X) dist
        # compute average velocity (dist weighted?)
        # approach that velocity

        pass


    
    new_pos = 0.001*new_vel + position

    return new_pos, new_vel

def update_plot():
    while dpg.is_dearpygui_running():
        global position
        global velocity
        position, velocity = update_flock(position, velocity)

        x_data = position[:,0].tolist()
        y_data = position[:,1].tolist()
        dpg.set_value("scatter_series", [x_data, y_data])
        #time.sleep(0.01)  # Adjust speed of animation

dpg.create_context()
dpg.create_viewport(title='Animated Scatter Plot', width=600, height=600)
dpg.setup_dearpygui()
dpg.show_viewport()


with dpg.window(label="Scatter Plot Animation", width=-1, height=-1, tag="main_window"):

    with dpg.theme(tag="plot_theme"):
        with dpg.theme_component(dpg.mvScatterSeries):
            dpg.add_theme_style(dpg.mvPlotStyleVar_MarkerSize, 0.8, category=dpg.mvThemeCat_Plots)


    with dpg.plot(label="Scatter Plot", height=-1, width=-1):
        dpg.add_plot_axis(dpg.mvXAxis, label="X Axis")
        y_axis = dpg.add_plot_axis(dpg.mvYAxis, label="Y Axis")
        dpg.add_scatter_series([], [], label="Data", parent=y_axis, tag="scatter_series")
        dpg.bind_item_theme("scatter_series", "plot_theme")

dpg.set_primary_window("main_window", True)

import threading
threading.Thread(target=update_plot, daemon=True).start()

dpg.start_dearpygui()
dpg.destroy_context()
