#include "glad.h"
#include "glfw3.h"

#include "obj/shader_constants.h"

#include <stdio.h>
#include <math.h>
#include <string.h>

const int resX = 1000;
const int resY = 1000;

void error_callback(int error, const char* description) {
    printf("Error %d: %s\n", error, description);
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
void checkError() {
    int error = glGetError();
    if(error != 0) {
        printf("Error found! Code: %d\n", error);
        exit(1);
    }
}

void draw(unsigned int program, unsigned int VAO) {
    float time = glfwGetTime();
    float color = pow(cos(time), 2.0);

    glClearColor(color, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    int vertexColorLocation = glGetUniformLocation(program, "our_color");

    glUseProgram(program);

    glUniform3f(vertexColorLocation, color, color, color);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // glFlush();
}

GLFWwindow* init() {
    if(!glfwInit()) {
        printf("GLFW failed to initialize!\n");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(resX, resY, "Tunnel", NULL, NULL);
    if(!window) {
        printf("Window failed to create!\n");
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
		printf("Vertex shader failed to compile!\n");
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
		printf("Fragment shader failed to compile.\n");
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
		printf("Shader program failed to link.");
		glDeleteShader(shaderProgram);
	}

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void dataInit(unsigned int* VAO) {
    float data[] = {
    //  vertices colors 
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f
    };
    unsigned int VBO;

    glGenVertexArrays(1, VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(*VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);
}

int main() {
    GLFWwindow* window = init();
    unsigned int program = programInit();
    unsigned int VAO;
    dataInit(&VAO);

    while(!glfwWindowShouldClose(window)) {
        draw(program, VAO);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();

}