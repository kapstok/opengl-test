#include "bg.hpp"
#include "glitter.hpp"

static const char* vert_shader = R"(
    #version 330 core

    out vec2 v_uv;
    void main() {
        uint idx = uint(gl_VertexID);
        gl_Position = vec4(idx & 1U, idx >> 1U, 0.0, 0.5) * 4.0 - 1.0;
        v_uv = vec2(gl_Position.xy * 0.5 + 0.5);
    }
)";

static const char* frag_shader = R"(
    #version 330 core

    uniform vec4 top_color;
    uniform vec4 bottom_color;
    in vec2 v_uv;
    out vec4 frag_color;

    void main() {
        frag_color = bottom_color * (1 - v_uv.y) + top_color * v_uv.y;
    }
)";

void gradient_bg(
    float top_r, float top_g, float top_b, float top_a,
    float bottom_r, float bottom_g, float bottom_b, float bottom_a
) {
        glDisable(GL_DEPTH_TEST);

        static GLuint vao = 0;
        static GLuint shaderProg = 0;

        if (vao == 0) {
            glGenVertexArrays(1, &vao);

            GLuint vs_id, fs_id;
            vs_id = glCreateShader(GL_VERTEX_SHADER);
            fs_id = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(vs_id, 1, &vert_shader, NULL);
            glShaderSource(fs_id, 1, &frag_shader, NULL);
            glCompileShader(vs_id);
            glCompileShader(fs_id);
            shaderProg = glCreateProgram();
            glAttachShader(shaderProg, vs_id);
            glAttachShader(shaderProg, fs_id);
            glLinkProgram(shaderProg);
            glDetachShader(shaderProg, fs_id);
            glDetachShader(shaderProg, vs_id);
            glDeleteShader(fs_id);
            glDeleteShader(vs_id);
            glUseProgram(shaderProg);
        }

        glUseProgram(shaderProg);
        GLuint top_color_loc = glGetUniformLocation(shaderProg, "top_color");
        GLuint bot_color_loc = glGetUniformLocation(shaderProg, "bottom_color");
        glUniform4f(top_color_loc, top_r, top_g, top_b, top_a);
        glUniform4f(bot_color_loc, bottom_r, bottom_g, bottom_b, bottom_a);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glEnable(GL_DEPTH_TEST);
}