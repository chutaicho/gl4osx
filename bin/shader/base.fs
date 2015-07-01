#version 410 core

uniform sampler2D tex1;
uniform sampler2D tex2;

//uniform float g_time;
uniform float g_sin;
uniform vec2  mouse;
uniform vec2  resolution;

in vec3 i_color;
in vec3 i_position;
in vec2 i_texcoord;

out vec4 FragColor;

void main()
{
    // //vec2 m = vec2(mouse.x, mouse.y);
    // //vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);
    // vec2 p = (gl_FragCoord.xy*2.0 + resolution) / min(resolution.x, resolution.y);
    // //vec2 v = vec2(0.0, 1.0);
    // //float t = dot(p, v) / (length(p) * length(v));
    // float t = (0.2 / length(mouse - p)) * 1.0;

    // vec3 col = vec3(0, 0, 0);

    // if(int(gl_FragCoord.x) < 10 && int(gl_FragCoord.y) > int(resolution.y*2.0-10.0))
    // {
    //     col = vec3(1.0, 0.0, 0.0);
    // }
    // col += vec3(t);

    // float distanceFromCenter = distance(mouse, resolution);
    // float checkForPresenceWithinCircle = step(distanceFromCenter, 0.1);

    vec4 texColor = texture(tex2, i_texcoord);

    // FragColor = vec4(vec3(t), 1.0);
    // FragColor = vec4(1.0,0.0,0.0,1.0) * checkForPresenceWithinCircle;
    // FragColor = mix(texture(tex1, i_texcoord), texture(tex2, i_texcoord),.5);
    FragColor = vec4(i_color,1.0);
}