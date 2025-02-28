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

uniform float a;
uniform float k;
uniform float mu;
uniform vec3 rayleigh_color;

void main()
{
	vec3 norm = texture(normal_map, TexCoord).xyz*2 - 1;

	vec3 light_dir_norm = normalize(light_dir);
	float light = 0.25 + 0.75*dot(light_dir_norm, norm);//modified lambertian

	float light_avg = 0.25 + 0.75*dot(light_dir_norm, vec3(0,0,1));

	vec3 terrain_color = terrain_color + terrain_color*terrain_intensity*(light-light_avg);

	vec4 unproj_pos = inv_view_projection * vec4(Position);
	unproj_pos  = unproj_pos / unproj_pos.w;
	vec3 world_pos = unproj_pos.xyz;
	float dist = length(camera_position-world_pos);

	//FragColor =  vec4(dist/100, 0, 0, 1.0);

	float add_color = 0.0;

	float radius = 0;
	int steps = 100;
	float intensity = 0.0;
	for (int i = 0; i < steps; i++)
	{
		radius += dist/steps;

		float volume = (dist/steps);
		intensity += 0.001*(0.5+a)*exp(-mu*radius)*volume * 1 / pow(radius, 2*k);
	}

	terrain_color += rayleigh_color*intensity;

	vec3 line_color = vec3(0,0,0.4);
	float spacing = 1000.0;
	vec2 xy_mod = mod(TexCoord*3601.0, spacing) - spacing/2.0;

	float k=5.0;
	float line_intensity = max(exp(-k*(xy_mod.x*xy_mod.x)),exp(-k*(xy_mod.y*xy_mod.y)));
	
	FragColor = vec4(terrain_color + line_color*line_intensity, 1.0);
}