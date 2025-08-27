import numpy as np

pos = []

f = 3e9
c = 3e8
_lambda = c/f

delta_x = delta_y = _lambda*0.5

for i in range(40):
    for j in range(40):
        pos.append(np.array([0, i*delta_x, j*delta_y]))

pos = np.array(pos)

k = (2*np.pi)/_lambda

def spherical_to_cartesian(v_spherical):
    rho = v_spherical[0]
    theta = v_spherical[1]
    phi = v_spherical[2]

    x = rho*np.sin(phi)*np.cos(theta)
    y = rho*np.sin(phi)*np.sin(theta)
    z = rho*np.cos(phi)

    return np.array([x, y, z])

def array_factor(theta, phi, k, rN, wN):
    k_vec = k*spherical_to_cartesian([1, theta, phi])
    k_dot_rN = (k_vec.T @ rN.T).T
    AF_vec = wN*np.exp(-1j*k_dot_rN)
    return np.sum(AF_vec)

def sample_rad_pattern(k, rN, wN, theta_phi_arr):

    AF = np.zeros(theta_phi_arr.shape[0], np.complex128)

    for i,(theta,phi) in enumerate(theta_phi_arr):
        AF[i] = array_factor(theta, phi, k, rN, wN)

    return AF

theta_phi_arr = np.meshgrid(np.deg2rad(np.linspace(-90, 90, 1024)),
                            np.deg2rad(np.linspace(0, 180, 1024)))

theta_phi_arr = np.array(theta_phi_arr).reshape((2, -1)).T

AF = sample_rad_pattern(k, pos, np.ones(pos.shape[0]), theta_phi_arr)

import plotly.express as px
from plotly.subplots import make_subplots
import plotly.graph_objects as go

import plotly.io as pio
pio.renderers.default = "browser"

AF_db = 20*np.log10(np.abs(AF))
AF_db -= AF_db.max()
AF_db = np.maximum(-50, AF_db)

AF_db -= AF_db.min()

#fig = make_subplots()
#fig.add_trace(go.Heatmap(z=AF_db, x=theta_phi_arr[:,0], y=theta_phi_arr[:,1]))
#fig.show()

AF_db = AF_db.reshape((1024, 1024))

import matplotlib.pyplot as plt
plt.imsave("grayscale_image.png", 255*AF_db/AF_db.max(), cmap='gray', vmin=0, vmax=255)