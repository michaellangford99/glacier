#version 330 core
out vec4 FragColor;
 
in vec4 Position;
in vec4 WorldPosition;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Color;

uniform sampler2D normal_map;

uniform mat4 inv_view_projection;
uniform vec3 camera_position;

uniform vec3 terrain_color;
uniform float terrain_intensity;
uniform vec3 light_dir;

void main()
{
	vec3 norm = texture(normal_map, TexCoord).xyz*2 - 1;

	vec3 light_dir_norm = normalize(light_dir);
	float light = 0.25 + 0.75*dot(light_dir_norm, norm);//modified lambertian

	float light_avg = 0.25 + 0.75*dot(light_dir_norm, vec3(0,0,1));

	vec3 line_color = vec3(0,0,0.4);
	float spacing = 1000.0;
	vec2 xy_mod = ((TexCoord*3601.0) % spacing) - spacing/2.0;

	float k=5.0;
	float line_intensity = max(exp(-k*(xy_mod.x*xy_mod.x)),exp(-k*(xy_mod.y*xy_mod.y)));
	
	FragColor = vec4(terrain_color + terrain_color*terrain_intensity*(light-light_avg) + line_color*line_intensity, 1.0);
}