#include "glad.h"
#include "glfw3.h"

#include "obj/shader_constants.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int resX = 1000;
const int resY = 1000;
float vel = 0.0f;
float pos = 0.0f;

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error %d: %s\n", error, description);
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void inputs(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vel+=0.0001f;
    } else if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vel-=0.0001f;
    }
}

void checkError() {
    int error = glGetError();
    if(error != 0) {
        fprintf(stderr, "Error found! Code: %d\n", error);
        exit(1);
    }
}

void draw(unsigned int program, unsigned int VAO, unsigned int texture) {
    float time = glfwGetTime();
    pos+=vel;
    glClearColor(0.0, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    int resolutionLocation = glGetUniformLocation(program, "resolution");
    int timeLocation = glGetUniformLocation(program, "time");
    int posLocation = glGetUniformLocation(program, "pos");
    int velLocation = glGetUniformLocation(program, "vel");

    glUseProgram(program);

    glUniform1f(posLocation, pos);
    glUniform1f(velLocation, vel);
    glUniform1f(timeLocation, time);
    glUniform2i(resolutionLocation, resX, resY);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // glFlush();
}

GLFWwindow* init() {
    if(!glfwInit()) {
        fprintf(stderr, "GLFW failed to initialize!\n");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(resX, resY, "Tunnel", NULL, NULL);
    if(!window) {
        fprintf(stderr, "Window failed to create!\n");
    }

    glfwSetErrorCallback(error_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1);

    glfwSetTime(0.0);
    return window;
}

unsigned int programInit() {
    const char *vertex = (char*) shaders_vertex_glsl;
    const char *fragment = (char*) shaders_fragment_glsl;

    // vertex shader

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex, NULL);
    glCompileShader(vertexShader);

    int success;
	char log[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    glGetProgramInfoLog(vertexShader, 512, NULL, log);
    printf("Vertex shader info log: %s\n", log);
	if (!success) {
		fprintf(stderr, "Vertex shader failed to compile!\n");
		glDeleteShader(vertexShader);
	}

    // Fragment shader

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment, NULL);
    glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    glGetProgramInfoLog(fragmentShader, 512, NULL, log);
    printf("Fragment shader info log: %s\n", log);
	if (!success) {
		fprintf(stderr, "Fragment shader failed to compile.\n");
		glDeleteShader(fragmentShader);
	}

    // Shader program

    unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindAttribLocation(shaderProgram, 0, "vertices");
	glBindAttribLocation(shaderProgram, 0, "colors");
	glLinkProgram(shaderProgram);

    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
    glGetProgramInfoLog(shaderProgram, 512, NULL, log);
    printf("Program info log: %s\n", log);
	if (!success) {
		fprintf(stderr, "Shader program failed to link.\n");
		glDeleteShader(shaderProgram);
	}

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void dataInit(unsigned int* VAO) {
    float data[] = {
    //  vertices     colors            texcoords
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // tr
        1.0f, -1.0f, 0.0f, 0.0f, 0.f, 1.0f, 0.0f, // br
        -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bl
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f // tl
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    unsigned int VBO, EBO;

    glGenVertexArrays(1, VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(*VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(5*sizeof(float)));
    glEnableVertexAttribArray(2);
}

unsigned int genTextures() {
    int width, height, channelCount;
    unsigned char *data = stbi_load("image.png", &width, &height, &channelCount, 0);

    if(!data) {
        fprintf(stderr, "Failed to load texture");
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return texture;
}

int main() {
    GLFWwindow* window = init();
    unsigned int program = programInit();
    unsigned int VAO;
    dataInit(&VAO);
    unsigned int texture = genTextures();

    while(!glfwWindowShouldClose(window)) {
        inputs(window);

        draw(program, VAO, texture);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

}