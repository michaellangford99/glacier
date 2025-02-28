#version 330 core
out vec4 FragColor;
 
in vec4 Position;
in vec4 WorldPosition;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Color;

uniform float iTime;

uniform mat4 inv_view_projection;
uniform vec3 camera_position;

// Determine the unit vector to march along
vec3 rayDirection(in float fieldOfView, in vec2 size, in vec2 frag_coord) {
    vec2 xy = frag_coord - size / 2.0;
    float z = size.y / tan(fieldOfView / 2.0);
    return normalize(vec3(xy, -z));
}

float evaluate_equation(vec3 pos)
{
    //float iTime= 0.0f;
    vec3 e1 = vec3(3.0+2.0*cos(iTime), 2.0+2.0*cos(iTime), 5.0+2.0*cos(iTime));
    vec3 e2 = vec3(-3, 0, 4);
    
    float tdoa = 3.0+2.0*sin(iTime);
    
    float d1 = abs(length(pos-e1)-length(pos-e2) - tdoa);
    float d2 = abs(length(pos-e1)-length(pos-e2) + tdoa);
    float d = min(d1, d2);
    
    //return length(pos-e1)-2.0;
    
    return length(pos-e1)-length(pos-e2) + tdoa;//d;
}

vec3 gradient(vec3 pos)
{
    float delta = 0.01;
    
    float origin = evaluate_equation(pos);
    
    float dx = evaluate_equation(pos+vec3(delta,0,0))-origin;    
    float dy = evaluate_equation(pos+vec3(0,delta,0))-origin;    
    float dz = evaluate_equation(pos+vec3(0,0,delta))-origin;
    
    return vec3(dx, dy, dz)/delta;
}

mat3 hessian(vec3 pos)
{
    //must evaluate a matrix of partial derivatives.
    //can be seen as the transposed jacobian of the gradient right?
    //which for range space of R1 means del`*del(f) yeah?
    float delta = 0.01;
    
    vec3 origin = gradient(pos);
    
    vec3 dx = gradient(pos+vec3(delta,0,0))-origin;
    vec3 dy = gradient(pos+vec3(0,delta,0))-origin;
    vec3 dz = gradient(pos+vec3(0,0,delta))-origin;
    
    mat3 H = transpose(mat3(dx, dy, dz))/delta;
    return H;
}

float raymarch_hyperbola_newton(vec3 eye, vec3 ray_dir, out bool success)
{
    success = false;
    
    vec3 pos = eye;
    
    for (int i = 0; i < 50; i++)
    {
        vec3 grad = gradient(pos);
        float dir_1st_deriv = -length(grad);//dot(grad,ray_dir);
    
        mat3 H = hessian(pos);
        float dir_2nd_deriv = dot(ray_dir*H, ray_dir);
    
        pos += (-evaluate_equation(pos)/dir_1st_deriv)*ray_dir;
        //pos += (-dir_1st_deriv/dir_2nd_deriv)*ray_dir;
        
        //evaluate how close we are to satisfying the eq
        float d = evaluate_equation(pos);
  
        if (abs(d) < .05) {
            //if (length(pos) < 5.0) {
            success = true;
            return length(pos-eye);
            //}
        }
    }
}

vec3 normal(vec3 pos)
{
    return normalize(gradient(pos));
}

float raymarch_hyperbola(vec3 eye, vec3 ray_dir, out bool success)
{
    //float iTime = 0.0f;

    vec3 e1 = vec3(3.0+2.0*cos(iTime), 2.0+2.0*cos(iTime), 4.0+2.0*cos(iTime));
    vec3 e2 = vec3(-3, 0, 4);
    
    float tdoa = 3.0+2.0*sin(iTime);
    
    float delta = 0.06;
    
    success = false;
    
    vec3 pos = eye;
    
    for (int i = 0; i < 100; i++)
    {
        delta += 0.006;
        pos += delta*ray_dir;
        //evaluate how close we are to satisfying the eq
        float d = evaluate_equation(pos);
    
        if (d < 0.1) {
            //if (length(pos) < 15.0) {
            success = true;
            return length(pos-eye);
            //}
        }
    }
}

void main() {

    vec4 unproj_pos = inv_view_projection * vec4(Position.xy, 1, 1);
	unproj_pos  = unproj_pos / unproj_pos.w;
	vec3 world_pos = unproj_pos.xyz;

	vec3 ray = normalize(world_pos - camera_position);

    vec3 ray_dir = ray;//rayDirection(FOV, iResolution.xy, fragCoord);
    vec3 eye = camera_position;
    bool success = false;
    float dist = raymarch_hyperbola_newton(eye, ray_dir, success);

    // The closest point on the surface to the eyepoint along the view ray
    vec3 p = eye + dist * ray_dir;

    if (p.z < 0) discard;

    if (success == false) {
        // Nothing was hit
        // Create nice gradient effect for the background
        //fragColor = vec4(0.0, .8-sqrt(ray_dir.y * 5.0), .8-sqrt(ray_dir.y * 5.0), 1.0);
        discard;
    } else {
        // Something was hit
        vec3 n = normal(p);
        vec3 ldir = normalize(vec3(0.5,2,1));
        float l = pow(dot(ldir, n)*0.5+0.5, 2.0);
        
        //mat3 H = hessian(p);
        
        //float dir_2nd_deriv = dot(ray_dir*H, ray_dir);
        
        FragColor = vec4(vec3(1, 0, 0)*1.0-dist/40.0, 0.5);
    }

}