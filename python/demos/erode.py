#%%

import dearpygui.dearpygui as dpg
import random
import time

import numpy as np

#N = 10000
#height = np.ndarray((N,N))

height = np.fromfile("../../content/N40W106.hgt", dtype='>i2')

height = height.reshape((3601, -1))

height = height[400:800,700:1000]

from scipy import interpolate

#height = np.repeat(height, 10, 0)
#height = np.repeat(height, 10, 1)

import matplotlib.pyplot as plt

plt.imshow(height)
plt.show()

#def compute_gradient(height, x_idx, y_idx):
#    dx = height[x_idx+1,y_idx] - height[x_idx,y_idx]
#    dy = height[x_idx,y_idx+1] - height[x_idx,y_idx]

#    return np.array([dx, dy])

#ok so actually just compute as a vector operation the gradient of the whole heightfield

#for i in range(height.shape[0]):
#    for j in range(height.shape[1]):

dxdy = np.zeros((height.shape[0], height.shape[1], 2))

dxdy[1:,:,0] = height[:-1,:] - height[1:,:]
dxdy[:,1:,1] = height[:,:-1] - height[:,1:]

mag_grad = np.abs(dxdy[:,:,0] + dxdy[:,:,1]*1j)

fig, axs = plt.subplots(1, 3)

print(axs)

axs[0].imshow(dxdy[1:,:,0])
axs[1].imshow(dxdy[:,1:,1])
axs[2].imshow(mag_grad)
plt.show()

water = np.ones(height.shape)

# %%

for i in range(10):

    new_water = np.zeros(height.shape)

    for x in range(height.shape[0]):
        for y in range(height.shape[1]):
            #first move this water cell in direction of its gradient
            factor=1#0.1
            delta = dxdy[x,y,:]*factor
            delta = delta/(0.1+np.abs(dxdy[x,y,0] + dxdy[x,y,1]*1j))
            if 0 <= np.round(x+delta[0]) < height.shape[0]:
                if 0 <=  np.round(y+delta[1]) < height.shape[1]:
                    new_water[np.round(x+delta[0]).astype(int), np.round(y+delta[1]).astype(int)] += water[x,y]

    water = new_water

    plt.imshow((water))
    plt.show()
# %%
