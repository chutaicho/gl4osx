#version 410 core

uniform sampler2D tex1;

uniform float g_time;
uniform float g_sin;
uniform vec2  mouse;
uniform vec2  resolution;

in vec3 i_color;
in vec3 i_position;
in vec2 i_texcoord;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(tex1, i_texcoord);
    FragColor = texColor;
}