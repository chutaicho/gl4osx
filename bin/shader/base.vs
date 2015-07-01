#version 410 core

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vc;
layout(location = 2) in vec2 vt;
layout(location = 3) in vec3 offset;
//layout(location = 3) in mat4 instanceMatrix;

out vec3 i_color;
out vec3 i_position;
out vec2 i_texcoord;

uniform float g_time;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 rotation;

mat4 rotate_z(float angle)
{
    return mat4(cos(angle), -sin(angle), 0.0, 0.0,
                sin(angle),  cos(angle), 0.0, 0.0,
                0.0,         0.0, 1.0, 0.0,
                0.0,         0.0, 0.0, 1.0);
}

mat4 rMatrix = rotate_z(-g_time);

void main(void)
{
	i_color = vc;
	i_position = vp;
	i_texcoord = vt;
    
	//vec3 offset = offsets[gl_InstanceID];
    //gl_Position = vec4(vp + offset, 0.0, 1.0);
    float resize = .05;
    
    vec4 pos = vec4(vp, 1.0) * rotate_z(-g_time);
    gl_Position = projection * view * model * (pos * resize + vec4(vec3(offset.x,offset.y,offset.z), 1.0));
}