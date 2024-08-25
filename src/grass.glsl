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

uniform float lean;
uniform float offset;
//uniform vec3 grass_color;

void main()
{
    vec3 grass_color = vec3(0.0,1.0,0.0);

    float mask = texture(texture1, TexCoord).r;

    float alpha = 1.0;
    float ht = mask*(texture(texture0, TexCoord*4.0 + vec2(WorldPosition.z*lean, 0)).r-(0.3+offset))/(1.0-(0.3+offset));
    float h = WorldPosition.z*10.0;
    if (h > ht) alpha = 0.0;//1.0 - (h-ht)*20.00;

	FragColor = vec4(grass_color*h, alpha);
}