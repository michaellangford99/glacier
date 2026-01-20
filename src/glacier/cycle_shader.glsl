#version 330 core
out vec4 FragColor;
 
in vec4 Position;
in vec4 WorldPosition;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Color;


uniform sampler2D input_map;
uniform sampler2D test_map;
uniform int reset;

vec2 compute_gradient(int x, int y)
{
    float dddx = 1.0;
    vec2 coord = vec2(x/2048.0, y/2048.0);
    float h = texture(test_map, coord).x;
    float hx = texture(test_map, coord+vec2(dddx/2048.0, 0.0)).x;
    float hy = texture(test_map, coord+vec2(0.0, dddx/2048.0)).x;

    vec3 dx = normalize(vec3(1.0/2048.0, 0, hx-h));
    vec3 dy = normalize(vec3(0, 1.0/2048.0, hy-h));

    vec3 norm = normalize(cross(dx, dy));

    return vec2(norm.x, norm.y);
}

float get_water(int x, int y)
{
    vec2 coord = vec2(x/2048.0, y/2048.0);
    float original_water = texture(input_map, coord).x;
}

void main() {

    float ddddddx = 1.0/2048.0;
    float test = texture(test_map, TexCoord).x;
    float original_water = texture(input_map, TexCoord).x;

    if (reset > 0.0)
        FragColor = vec4(0.01, 1, 1, 1);
    else
    {
        float dddx = 1.0;
        float h = texture(test_map, TexCoord).x;
        float hx = texture(test_map, TexCoord+vec2(dddx/2048.0, 0.0)).x;
        float hy = texture(test_map, TexCoord+vec2(0.0, dddx/2048.0)).x;

        vec3 dx = normalize(vec3(1.0/3601.0, 0, hx-h));
        vec3 dy = normalize(vec3(0, 1.0/3601.0, hy-h));

        vec3 norm = normalize(cross(dx, dy));

        //grab water from opposite gradient direction
        float water = texture(input_map, TexCoord+46*ddddddx*vec2(norm.x, norm.y)).x;

        FragColor = vec4(original_water+water, 1, 1, 1);
    }

}