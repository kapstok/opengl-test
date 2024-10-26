// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <string>
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const char* vertexSource = R"(
#version 330
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

layout(location = 0) in vec3 vert;
layout(location = 1) in vec3 vertNormal;

out vec3 fragVert;
out vec3 fragNormal;

void main() {
    fragNormal = vertNormal;
    fragVert = vert;
    gl_Position = projection * view * model * vec4(vert, 1);
}
)";

static const char* fragSource = R"(
#version 330

uniform mat4 model;
uniform struct Light {
   vec3 position;
   vec3 intensities;
} light;
uniform vec3 ambient;

in vec3 fragNormal;
in vec3 fragVert;
out vec4 fragColor;

void main() {
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 normal = normalize(normalMatrix * fragNormal);
    vec3 fragPosition = vec3(model * vec4(fragVert, 1));
    vec3 surfaceToLight = light.position - fragPosition;
    float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
    brightness = clamp(brightness, 0, 1);
    vec3 diffuse = brightness * light.intensities;
    fragColor = vec4(diffuse + ambient, 1);
}
)";

int main(int argc, char * argv[]) {
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    glEnable(GL_DEPTH_TEST);

    GLfloat vertexData[] = {
        // x, y, z, normalx, normaly, normalz
        // bottom
        -1.0f,-1.0f,-1.0f, 0.0f, -1.0f, 0.0f,
         1.0f,-1.0f,-1.0f, 0.0f, -1.0f, 0.0f,
        -1.0f,-1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
         1.0f,-1.0f,-1.0f, 0.0f, -1.0f, 0.0f,
         1.0f,-1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        -1.0f,-1.0f, 1.0f, 0.0f, -1.0f, 0.0f,

        // top
        -1.0f, 1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         1.0f, 1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
         1.0f, 1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        // front
        -1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         1.0f,-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        // back
        -1.0f,-1.0f,-1.0f, 0.0f, 0.0f, -1.0f,
        -1.0f, 1.0f,-1.0f, 0.0f, 0.0f, -1.0f,
         1.0f,-1.0f,-1.0f, 0.0f, 0.0f, -1.0f,
         1.0f,-1.0f,-1.0f, 0.0f, 0.0f, -1.0f,
        -1.0f, 1.0f,-1.0f, 0.0f, 0.0f, -1.0f,
         1.0f, 1.0f,-1.0f, 0.0f, 0.0f, -1.0f,

        // left
        -1.0f,-1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f,-1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f,-1.0f,-1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f,-1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f,-1.0f, -1.0f, 0.0f, 0.0f,

        // right
         1.0f,-1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         1.0f,-1.0f,-1.0f, 1.0f, 0.0f, 0.0f,
         1.0f, 1.0f,-1.0f, 1.0f, 0.0f, 0.0f,
         1.0f,-1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         1.0f, 1.0f,-1.0f, 1.0f, 0.0f, 0.0f,
         1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f
    };

    GLuint vbo, vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // Enable vertex attributes for position and normal
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Compile and link shaders
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSource, nullptr);
    glCompileShader(fragShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    // Uniform locations
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    GLint lightPosLoc = glGetUniformLocation(shaderProgram, "light.position");
    GLint lightIntLoc = glGetUniformLocation(shaderProgram, "light.intensities");
    GLint ambientLoc = glGetUniformLocation(shaderProgram, "ambient");

    // Main rendering loop
    while (!glfwWindowShouldClose(mWindow)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        // Set up the view, projection, and model matrices
        glm::mat4 view = glm::lookAt(glm::vec3(4.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)mWidth / mHeight, 0.1f, 10.0f);
        glm::mat4 model = glm::mat4(1.0f);

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Set up lighting
        glUniform3f(lightPosLoc, 2.0f, 5.0f, 5.0f);
        glUniform3f(lightIntLoc, 252.0f / 255.0f, 187.0f / 255.0f, 75.0f / 255.0f);
        glUniform3f(ambientLoc, 51.0f / 255.0f, 35.0f / 255.0f, 47.0f / 255.0f);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(mWindow);
    glfwTerminate();
    return EXIT_SUCCESS;
}
