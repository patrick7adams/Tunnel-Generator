#version 330 core
out vec4 FragColor;

uniform ivec2 resolution;
uniform float time;
uniform float vel;
uniform float pos;
uniform sampler2D tex;

in vec3 new_colors;
in vec2 texCoord;

void main() {
    vec2 uv = (gl_FragCoord.xy/resolution)-vec2(0.5, 0.5);
    // vec2 uv = (gl_FragCoord.xy - 0.5*resolution)/ resolution.y;
    float dist = length(uv);
    float wackyDist = pow(uv.x*uv.x+uv.y*uv.y, -2.5*vel);
    float angle = atan(uv.y, uv.x);

    vec2 cool = vec2((angle/3.14159)+sin(time)/5.0, 0.1/dist+pos);
    FragColor = texture(tex, cool)-vec4(1.0-wackyDist, 1.0-wackyDist, 1.0-wackyDist, 1.0);
}