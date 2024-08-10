#version 330 core
out vec4 FragColor;
 
in vec4 Position;
in vec4 WorldPosition;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Color;

uniform sampler2D texture0;
uniform sampler2D texture1;

uniform mat4 inv_view_projection;
uniform vec3 camera_position;
//uniform float eps;

float circle_sdf(vec3 pos, vec3 center, float radius)
{
	return length(pos-center)-radius;
}

// repeat space every s units
float repeated( vec3 p, float s)
{
    vec3 r = p - s*vec3(round(p.xy/s),0);
	vec2 index = round(p.xy/s);
	vec2 offset = fract(vec2(index.x/3.1415963 + index.y/2.707 + index.y*index.x/2.7, index.x/35.4353 + index.y/1.55 + index.y*index.x/1.2))*0.3;
    return circle_sdf(r, vec3(offset,WorldPosition.z), 0.3);
}

void main()
{
	//bump up WorldPosition

	vec3 pos = camera_position;

	int i = 0;

	int hit = 0;
	for (i=0; i<100; i++)
	{
		float dist = repeated(pos, 1.0);
		if (dist < 0.001) 
		{
			hit=1;
			break;
		}
		pos += normalize(WorldPosition.xyz-camera_position)*dist;
	}

	//FragColor = texture(texture0, TexCoord);
	FragColor = vec4(dot(normalize(vec3(0.4,0.3,1)), Normal)*vec3(1,1,1).xy, hit, 1.0);
}