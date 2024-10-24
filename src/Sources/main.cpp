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
    in vec3 color;
    out vec3 Color;
    void main()
    {
        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

static const char* fragSource = R"glsl(
    #version 150 core
    in vec3 Color;
    out vec4 outColor;
    void main()
    {
        outColor = vec4(Color, 1.0);
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

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Vertices. Starts syntax: x, y, r, g, b
    float vertices[] = {
        -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
    };
    GLuint vbo, vao, ebo;
    glGenBuffers(1, &vbo); // Generate 1 buffer
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);

    GLint colAttrib = glGetAttribLocation(shaderProg, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 2));

    // Tutorial about using uniforms
    //GLint uniColor = glGetUniformLocation(shaderProg, "color");
    //glUniform3f(uniColor, 1.0f, 0.5f, 0.2f);
    //auto t_start = std::chrono::high_resolution_clock::now();

    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        /*
        * len kan alleen berekend worden als de array in dezelfde scope zit.
        * Anders, als het als arg meegegeven wordt, wordt de array omgezet in
        * een pointer. Dan werkt het weer niet :/
        */
        size_t len = sizeof(elements) / sizeof(GLuint);
        glDrawElements(GL_TRIANGLES, len, GL_UNSIGNED_INT, 0);

        // Change color
        //auto t_now = std::chrono::high_resolution_clock::now();
        //float delta = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
        //glUniform3f(uniColor, (sin(delta * 4.0f) + 1.0f) / 2.0f, 0.5f, 0.2f);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    
    glDeleteProgram(shaderProg);
    glDeleteShader(fragShader);
    glDeleteShader(vertShader);

    glDeleteBuffers(1, &vbo);

    glDeleteVertexArrays(1, &vao);
    
    glfwTerminate();
    return EXIT_SUCCESS;
}
