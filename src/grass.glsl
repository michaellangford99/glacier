#version 330 core
out vec4 FragColor;
  
in vec4 Position;
in vec4 WorldPosition;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Color;

uniform sampler2D grass_texture;
uniform sampler2D mask_texture;

uniform mat4 inv_view_projection;
uniform vec3 camera_position;

uniform float lean;
uniform float lean_sq;
uniform float offset;
uniform float factor;

uniform float min_grass;
uniform vec3 grass_color;

uniform float tex_coord;
uniform float masktex_coord;
uniform float masktex_coord2;

uniform float lean_fact;
uniform float lean_fact2;

uniform float t;
uniform vec2 tcoffset;

void main()
{
    //vec3 grass_color = vec3(0.0,1.0,0.0);

    float mask = mix(0.6 + min_grass/10.0, 1.0, texture(mask_texture, masktex_coord*TexCoord).r);

    float alpha = 1.0;
    float ht = mask*(texture(grass_texture, TexCoord*tex_coord + vec2(WorldPosition.z*(lean+texture(mask_texture, masktex_coord2*TexCoord+t*tcoffset).r*lean_fact) + pow(WorldPosition.z*(lean_sq+texture(mask_texture, masktex_coord2*TexCoord+t*tcoffset).r*lean_fact2), 2.0), 0)).r-(0.3+offset))/(1.0-(0.3+offset));
    float h = WorldPosition.z*10.0;
    if (h >= ht) alpha = 0.0;//1.0 - (h-ht)*20.00;

	FragColor = vec4(grass_color*h*factor, alpha);
}