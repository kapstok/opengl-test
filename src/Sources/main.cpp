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

static const char* vertexSource = R"glsl(
    #version 150 core

    in vec2 position;

    void main() {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

static const char* fragSource = R"glsl(
    #version 150 core

    uniform vec3 triangleColor;
    out vec4 outColor;

    void main() {
        outColor = vec4(triangleColor, 1.0);
    }
)glsl";

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    // Generate a triangle
    float vertices[] = {
        0.0f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f
    };
    GLuint vbo, vao, ebo;
    glGenBuffers(1, &vbo); // Generate 1 buffer
    glGenVertexArrays(1, &vao);
    //glGenBuffers(1, &ebo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(vao);

    //glBindBuffer(GL_ARRAY_BUFFER, ebo);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
/*
    std::string vertPath = "default.vert";
    std::string fragPath = "default.frag";
    unsigned int shaderProg = createShaderProgram(vertPath.c_str(), fragPath.c_str());
    glUseProgram(shaderProg);
*/

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertexSource, NULL);
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragSource, NULL);
    glCompileShader(fragShader);

    // Check if succesfully compiled
    GLint status_v;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status_v);
    if (status_v == GL_FALSE) {
        char buffer[512];
        glGetShaderInfoLog(vertShader, 512, NULL, buffer);
        printf("Failed to compile vert shader:\n%s\n", buffer);
    }
    GLint status_f;
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status_f);
    if (status_f == GL_FALSE) {
        char buffer[512];
        glGetShaderInfoLog(fragShader, 512, NULL, buffer);
        printf("Failed to compile frag shader:\n%s\n", buffer);
    }

    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertShader);
    glAttachShader(shaderProg, fragShader);
    glBindFragDataLocation(shaderProg, 0, "outColor");
    glLinkProgram(shaderProg);
    glUseProgram(shaderProg);

    GLint posAttrib = glGetAttribLocation(shaderProg, "position");
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);

    // Tutorial about using uniforms
    GLint uniColor = glGetUniformLocation(shaderProg, "triangleColor");
    glUniform3f(uniColor, 1.0f, 0.5f, 0.2f);
    auto t_start = std::chrono::high_resolution_clock::now();

    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Change color
        auto t_now = std::chrono::high_resolution_clock::now();
        float delta = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
        glUniform3f(uniColor, (sin(delta * 4.0f) + 1.0f) / 2.0f, 0.5f, 0.2f);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    
    // TODO: Delete and detach shaders!
    
    glfwTerminate();
    return EXIT_SUCCESS;
}