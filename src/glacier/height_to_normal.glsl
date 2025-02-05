#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D height_map;

void main()
{
    vec2 im = texture(height_map, TexCoord).xy*1000.0;
	float dddx = 1.0;
	float h = texture(height_map, TexCoord).x;//*32768.0;
	float hx = texture(height_map, TexCoord+vec2(dddx/3601.0, 0.0)).x;//*32768.0;
	float hy = texture(height_map, TexCoord+vec2(0.0, dddx/3601.0)).x;//*32768.0;

	vec3 dx = normalize(vec3(1.0/3601.0, 0, hx-h));
	vec3 dy = normalize(vec3(0, 1.0/3601.0, hy-h));

	vec3 norm = normalize(cross(dx, dy));

	FragColor = vec4((norm+1)*0.5, 1.0);
}