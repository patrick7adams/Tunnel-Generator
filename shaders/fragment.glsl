#version 330 core
out vec4 FragColor;

in vec3 new_colors;
uniform vec3 our_color;

void main() {
    // FragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
    FragColor = vec4(new_colors+our_color, 1.0f);
}