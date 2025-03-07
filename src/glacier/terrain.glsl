#version 330 core
out vec4 FragColor;
 
in vec4 Position;
in vec4 WorldPosition;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Color;

uniform sampler2D normal_map;
uniform sampler2D height_map;

uniform mat4 inv_view_projection;
uniform vec3 camera_position;

uniform vec3 terrain_color;
uniform float terrain_intensity;
uniform vec3 light_dir;
uniform vec3 sunlight_dir;
uniform vec3 sun_color;

uniform float h_slice;

uniform float a;
uniform float k;
uniform float mu;
uniform vec3 rayleigh_color;

void main()
{
	vec3 norm = texture(normal_map, TexCoord).xyz*2 - 1;
	float height = texture(height_map, TexCoord).x*65535.0;

	vec3 light_dir_norm = normalize(light_dir);
	float light = 0.25 + 0.75*dot(light_dir_norm, norm);//modified lambertian

	float sunlight = (dot(normalize(sunlight_dir), norm));//modified lambertian
	if (sunlight < 0)
		sunlight = 0;


	vec2 pos = TexCoord;
	int step_count = 100;
	for (int i = 0; i < step_count; i++)
	{
		vec3 march_dir = mat3(1, 0, 0,
						      0, 1, 0,
							  0, 0, 0) * light_dir_norm;

		pos += light_dir_norm.xy*0.0002;
		float s_height = texture(height_map, pos).x*65535.0;
		if (s_height > height+35)
			light = light*0.989;
	}


	//float light_avg = 0.25 + 0.75*dot(light_dir_norm, vec3(0,0,1));

	vec3 terrain_color = terrain_color*light + sun_color*sunlight;

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
	float line_intensity = 0;//max(exp(-k*(xy_mod.x*xy_mod.x)),exp(-k*(xy_mod.y*xy_mod.y)));
	

	FragColor = vec4(terrain_color + line_color*line_intensity, 1.0);
	if (height > h_slice)
		FragColor *= 1+(height-h_slice)/200;
}