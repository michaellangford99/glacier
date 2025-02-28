#version 330 core
out vec4 FragColor;
 
in vec4 Position;
in vec4 WorldPosition;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Color;

uniform vec3 debug_color;

//uniform sampler2D normal_map;

//uniform mat4 inv_view_projection;
//uniform vec3 camera_position;

//uniform vec3 terrain_color;
//uniform float terrain_intensity;
//uniform vec3 light_dir;

void main()
{
	FragColor = vec4(debug_color, 1.0);
}