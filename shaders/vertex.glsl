#version 330 core
layout (location = 0) in vec2 vertices;
layout (location = 1) in vec3 colors;
layout (location = 2) in vec2 texCoords;

out vec3 new_colors;
out vec2 texCoord;

void main() {
    gl_Position = vec4(vertices, 0.0, 1.0);
    new_colors = colors;
    texCoord = texCoords;
}