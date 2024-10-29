TODO:
font
3D renderable text

library & executable separation
include & library inclusion as subrepositories? 
- somehow link them to their origin but include them in the repo
line drawing utility
geometry and compute shader boilerplate

-----

shader
- singleton for caching the shader programs
- copy program and master copy of uniform table from shader program
- if recompile create new instead of use cached
x cache values of shader values that persist
x include textures and autoset textures
  x display texture via imgui
x implement set_uniform
  - esp for 4x4 matrix
- cache shader location indices

texture
- alternate forms of image load, allow null image load
- resize
  
framebuffer
x copy basic implementation

terrain
x precompute normal map via framebuffer
- generate depth map
- depth shader
  
volumetric rendering

onscreen text
- freetype loader
- queue 2d and 3d text
- opacity

error handling and explanations
move to higher version of glfw and glad for better data handling

debug draw - singleton
global imgui editor class
atmosphere
better terrain, remove terrain seams with positional indexing
blending
renderer
camera
- arc ball with set look_at
- pan
- callbacks that use dynamic pointer cast to allow callback into class

separate main.cpp and a main startup class
tree editor that generates imgui window for each element and its own shader as well
glfw window destroy

ideas:
- node editor
  - image loading in node editor
  - general system for viewing, editing, and printing openGL flags
  - for example, live editing blend modes or setup parameters for a particular framebuffer
  - then I can have a node graph of all the framebuffers
  - 
- model loading
- 