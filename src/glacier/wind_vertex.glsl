#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;

out vec4 Position;
out vec4 WorldPosition;
out vec3 Normal;
out vec2 TexCoord;
out vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec2 a;
uniform vec2 b;
uniform vec2 c;
uniform float z;
uniform float zz;
uniform vec2 dir;

void main()
{
	WorldPosition = model * vec4(aPos, 1.0);

    //modifications to WorldPosition:
    WorldPosition.xy = a*WorldPosition.xy + WorldPosition.xy*WorldPosition.xy*b + WorldPosition.xy*WorldPosition.xy*WorldPosition.xy*c;

    WorldPosition.xy += dir*(z*WorldPosition.z + zz*WorldPosition.z*WorldPosition.z);

    Position = projection * view * WorldPosition;
	gl_Position = Position;
	Normal = mat3(model) * aNormal;
    TexCoord = aTexCoord;
    Color = aColor;
}