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

//////////------------------------------------

uniform vec3 uSunPos;

#define PI 3.141592
#define iSteps 8
#define jSteps 3

vec2 rsi(vec3 r0, vec3 rd, float sr) {
    // ray-sphere intersection that assumes
    // the sphere is centered at the origin.
    // No intersection when result.x > result.y
    float a = dot(rd, rd);
    float b = 2.0 * dot(rd, r0);
    float c = dot(r0, r0) - (sr * sr);
    float d = (b*b) - 4.0*a*c;
    if (d < 0.0) return vec2(1e5,-1e5);
    return vec2(
        (-b - sqrt(d))/(2.0*a),
        (-b + sqrt(d))/(2.0*a)
    );
}

vec3 atmosphere(vec3 frag_pos, vec3 r, vec3 r0, vec3 pSun, float iSun, float rPlanet, float rAtmos, vec3 kRlh, float kMie, float shRlh, float shMie, float g) {
    // Normalize the sun and view directions.
    pSun = normalize(pSun);
    r = normalize(r);

    // Calculate the step size of the primary ray.
    vec2 p = rsi(r0, r, rAtmos);
    if (p.x > p.y) return vec3(0,0,0);
    p.y = min(p.y, rsi(r0, r, rPlanet).x);

	//ensure distance is less than terrain
	//would like to be able to call raycast equation for terrain lighting as well
	float sphere_hit_dist = (p.y - p.x);

	float terrain_hit_dist = length(r0 - frag_pos);

    float iStepSize = terrain_hit_dist / float(iSteps);

    // Initialize the primary ray time.
    float iTime = 0.0;

    // Initialize accumulators for Rayleigh and Mie scattering.
    vec3 totalRlh = vec3(0,0,0);
    vec3 totalMie = vec3(0,0,0);

    // Initialize optical depth accumulators for the primary ray.
    float iOdRlh = 0.0;
    float iOdMie = 0.0;

    // Calculate the Rayleigh and Mie phases.
    float mu = dot(r, pSun);
    float mumu = mu * mu;
    float gg = g * g;
    float pRlh = 3.0 / (16.0 * PI) * (1.0 + mumu);
    float pMie = 3.0 / (8.0 * PI) * ((1.0 - gg) * (mumu + 1.0)) / (pow(1.0 + gg - 2.0 * mu * g, 1.5) * (2.0 + gg));

    // Sample the primary ray.
    for (int i = 0; i < iSteps; i++) {

        // Calculate the primary ray sample position.
        vec3 iPos = r0 + r * (iTime + iStepSize * 0.5);

        // Calculate the height of the sample.
        float iHeight = length(iPos) - rPlanet;

        // Calculate the optical depth of the Rayleigh and Mie scattering for this step.
        float odStepRlh = exp(-iHeight / shRlh) * iStepSize;
        float odStepMie = exp(-iHeight / shMie) * iStepSize;

        // Accumulate optical depth.
        iOdRlh += odStepRlh;
        iOdMie += odStepMie;

        // Calculate the step size of the secondary ray.
        float jStepSize = rsi(iPos, pSun, rAtmos).y / float(jSteps);

        // Initialize the secondary ray time.
        float jTime = 0.0;

        // Initialize optical depth accumulators for the secondary ray.
        float jOdRlh = 0.0;
        float jOdMie = 0.0;

        // Sample the secondary ray.
        for (int j = 0; j < jSteps; j++) {

            // Calculate the secondary ray sample position.
            vec3 jPos = iPos + pSun * (jTime + jStepSize * 0.5);

            // Calculate the height of the sample.
            float jHeight = length(jPos) - rPlanet;

            // Accumulate the optical depth.
            jOdRlh += exp(-jHeight / shRlh) * jStepSize;
            jOdMie += exp(-jHeight / shMie) * jStepSize;

            // Increment the secondary ray time.
            jTime += jStepSize;
        }

        // Calculate attenuation.
        vec3 attn = exp(-(kMie * (iOdMie + jOdMie) + kRlh * (iOdRlh + jOdRlh)));

        // Accumulate scattering.
        totalRlh += odStepRlh * attn;
        totalMie += odStepMie * attn;

        // Increment the primary ray time.
        iTime += iStepSize;

    }

    // Calculate and return the final color.
    return iSun * (pRlh * kRlh * totalRlh + pMie * kMie * totalMie);
}

vec3 get_atmosphere_color()
{
	vec4 unproj_pos = inv_view_projection * vec4(Position);
	unproj_pos  = unproj_pos / unproj_pos.w;
	vec3 world_pos = unproj_pos.xyz;

	vec3 ray = normalize(world_pos - camera_position);

	vec3 earth_origin = vec3(0,0,6372e3);//planarization point of sphere of earth

    vec3 color = atmosphere(
		world_pos*1e3 + earth_origin,
		ray,           // normalized ray direction
		camera_position*1e3+earth_origin,               // ray origin
		uSunPos,                        // position of the sun
		22.0,                           // intensity of the sun
		6371e3,                         // radius of the planet in meters
		6471e3,                         // radius of the atmosphere in meters
		vec3(5.5e-6, 13.0e-6, 22.4e-6), // Rayleigh scattering coefficient
		21e-6,                          // Mie scattering coefficient
		8e3,                            // Rayleigh scale height
		1.2e3,                          // Mie scale height
		0.758                           // Mie preferred scattering direction
    );

    // Apply exposure.
    color = 1.0 - exp(-1.0 * color);

    return color;
}

//////////------------------------------------

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

	//false color terrain:
	//int p = 5;
	//if (texture(height_map, TexCoord+vec2(0.0001,0.0001)).x*65535.0 > height) p=1;
	//vec3 terrain_color = mix(terrain_color, vec3(0,0.6,0), pow(norm.z, p));

	//float light_avg = 0.25 + 0.75*dot(light_dir_norm, vec3(0,0,1));

	vec3 terrain_color = terrain_color*light + sun_color*sunlight;

	vec4 unproj_pos = inv_view_projection * vec4(Position);
	unproj_pos  = unproj_pos / unproj_pos.w;
	vec3 world_pos = unproj_pos.xyz;
	float dist = length(camera_position-world_pos);

	//FragColor =  vec4(dist/100, 0, 0, 1.0);

	float add_color = 0.0;

	float radius = 0;
	int steps = 20;
	float intensity = 0.0;
	for (int i = 0; i < steps; i++)
	{
		radius += dist/steps;

		float volume = (dist/steps);
		intensity += 0.001*(0.5+a)*exp(-mu*radius)*volume * 1 / pow(radius, 2*k);
	}

	//terrain_color += rayleigh_color*intensity;
	terrain_color += rayleigh_color*(1.0-exp(-dist*mu));

	terrain_color += get_atmosphere_color();

	vec3 line_color = vec3(0,0,0.4);
	float spacing = 1000.0;
	vec2 xy_mod = mod(TexCoord*3601.0, spacing) - spacing/2.0;

	float k=5.0;
	float line_intensity = 0;//max(exp(-k*(xy_mod.x*xy_mod.x)),exp(-k*(xy_mod.y*xy_mod.y)));
	

	FragColor = vec4(terrain_color + line_color*line_intensity, 1.0);
	if (height > h_slice+5000)
		FragColor *= 1+(height-(h_slice+5000))/200;
}