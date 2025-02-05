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

void main()
{
	Position = projection * view * model * vec4(aPos, 1.0);
	WorldPosition = model * vec4(aPos, 1.0);
    gl_Position = Position;
	Normal = mat3(model) * aNormal;
    TexCoord = aTexCoord;
    Color = aColor;
}