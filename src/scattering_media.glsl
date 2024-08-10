#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;
in vec4 Position;

uniform mat4 inv_view_projection;
uniform vec3 camera_position;

uniform vec3 light_pos;
//uniform vec3 dimensions;
uniform float radius;
uniform float attenuation;
uniform vec3 material_color;
uniform vec3 light_color;
uniform float step_size;
uniform int  step_power;
uniform float exposure;

float noise(float n)
{
	return abs(mod(pow(mod(sin(mod(pow(n, 3)+59, 113)*123+19), 29)*123+47, 2), 1.0));
}

vec3 noise(vec3 v)
{
	vec3 r = vec3(noise(v.x*v.y*v.z + mod(v.x + v.y + v.z, 0.1)), 0, 0);
	r = vec3(r.x, noise(r.x*v.y), 0);
	r = vec3(r.x, r.y, noise(r.y*v.z));
	return r;
}

void main()
{
    //FragColor = texture(ourTexture, TexCoord) * inputColor * vec4(ourColor, 1.0);

	//FragColor = vec4(TexCoord, 0.0, 1.0);

	//FragColor = inputColor*TexCoord.x;

	//FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), inputColor.g);

	vec4 pos = inv_view_projection * Position;

	vec4 unproj_pos = inv_view_projection * vec4(Position.xy, 1, 1);
	unproj_pos  = unproj_pos / unproj_pos.w;
	vec3 world_pos = unproj_pos.xyz;

	vec3 ray = normalize(world_pos - camera_position);
	vec3 march_pos = camera_position;

	//strategy
	/*
	use ray-circle intersection (or ray march if lazy) to get to surface
	then for each march Position
	march towards light position, integrating the amount of material encountered
	keep track of how much material encountered on ray inwards
	as both light scattered in to particle and light returned to viewer undergo exponential decay
	multi-bounce is too hard atm lol
	*/

	vec3 ray_sphere_intersection_point = vec3(0,0,0);

	vec3 out_scattering_march_pos = ray_sphere_intersection_point;

	int march_steps = 1000;

	FragColor = vec4(noise(world_pos), 1.0);
}