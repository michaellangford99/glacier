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

uniform float ddx;
//uniform float eps;

float hash( float n ) { return fract(sin(n)*753.5453123); }


//---------------------------------------------------------------
// value noise, and its analytical derivatives
//---------------------------------------------------------------

vec4 noised( in vec3 x )
{
    vec3 p = floor(x);
    vec3 w = fract(x);
	vec3 u = w*w*(3.0-2.0*w);
    vec3 du = 6.0*w*(1.0-w);
    
    float n = p.x + p.y*157.0 + 113.0*p.z;
    
    float a = hash(n+  0.0);
    float b = hash(n+  1.0);
    float c = hash(n+157.0);
    float d = hash(n+158.0);
    float e = hash(n+113.0);
	float f = hash(n+114.0);
    float g = hash(n+270.0);
    float h = hash(n+271.0);
	
    float k0 =   a;
    float k1 =   b - a;
    float k2 =   c - a;
    float k3 =   e - a;
    float k4 =   a - b - c + d;
    float k5 =   a - c - e + g;
    float k6 =   a - b - e + f;
    float k7 = - a + b + c - d + e - f - g + h;

    return vec4( k0 + k1*u.x + k2*u.y + k3*u.z + k4*u.x*u.y + k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z, 
                 du * (vec3(k1,k2,k3) + u.yzx*vec3(k4,k5,k6) + u.zxy*vec3(k6,k4,k5) + k7*u.yzx*u.zxy ));
}

float fbm(vec3 pos)
{
	float h = 0.0;
	for (int i = 0; i < 15; i++)
	{
		h += noised(pos*pow(2, i)).x/(pow(ddx, i));//ddx=0.9
	}
	return h/15;
}

void main()
{
	vec4 n  = noised(WorldPosition.xyz);

	//FragColor = vec4(TexCoord.xy, 0.0, 1.0);
	float mn = 0;
	float mx = (12000/3.0)/65535.0;

	vec2 im = texture(texture0, TexCoord).xy*1000.0;
	float dddx = 1.0;
	float h = texture(texture0, TexCoord).x;//*32768.0;
	float hx = texture(texture0, TexCoord+vec2(dddx/3601.0, 0.0)).x;//*32768.0;
	float hy = texture(texture0, TexCoord+vec2(0.0, dddx/3601.0)).x;//*32768.0;

	vec3 dx = normalize(vec3(1.0/3601.0, hx-h, 0));
	vec3 dy = normalize(vec3(0, hy-h, 1.0/3601.0));

	vec3 norm = normalize(cross(dy, dx));

	vec3 light_dir = normalize(vec3(1, 1, 1));
	float light = 0.25 + 0.75*dot(light_dir, norm);//half lambertian

	vec3 color = vec3(0.3608, 0.3608, 0.3608);

	vec3 line_color = vec3(0,0,0.4);
	float spacing = 1000.0;
	vec2 xy_mod = ((TexCoord*3601.0) % spacing) - spacing/2.0;

	float k=5.0;
	float line_intensity = max(exp(-k*(xy_mod.x*xy_mod.x)),exp(-k*(xy_mod.y*xy_mod.y)));

	FragColor = vec4(light*color + line_color*line_intensity, 1.0);
	//FragColor = vec4((h-2000.0)/1000.0, 0.0, 0.0, 1.0);
	//FragColor = vec4(dot(normalize(vec3(0.4,0.3,1)), Normal)*vec3(1,1,1).xy, hit, 1.0);
}