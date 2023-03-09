//
// Created by asuka on 09.03.2023.
//

# include <glad/glad.h>
# include <GLFW/glfw3.h>

# include <glm/vec3.hpp>
# include <glm/mat4x4.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>

# include <model.hpp>
# include <camera.hpp>

# include <cmath>
# include <iostream>

const char* vertex_shader_source =
        "#version 330 core\n"
        "layout (location = 0) in vec3 in_position;\n"
        "layout (location = 1) in vec3 in_color;\n"
        "uniform mat4 mvp;\n"
        "out vec3 color;\n"
        "void main() {\n"
        "   gl_Position = mvp * vec4(in_position, 1);\n"
        "   color = in_color;\n"
        "}";

const char* fragment_shader_source =
        "#version 330 core\n"
        "in vec3 color;"
        "out vec4 frag_color;\n"
        "void main() {\n"
        "   frag_color = vec4(color, 1);\n"
        "}";

struct Vertex {
    glm::vec3 pos;
    glm::vec3 col;
};

Vertex vertices[] = {
        {{1.f, 1.f, 1.f}, {0, 0, 0}},
        {{1.f, -1.f, 1.f}, {0, 1, 0}},
        {{-1.f, -1.f, 1.f}, {1, 0, 0}},
        {{-1.f, 1.f, 1.f}, {1, 1, 0}},

        {{1.f, 1.f, -1.f}, {0, 0, 1}},
        {{1.f, -1.f, -1.f}, {0, 1, 1}},
        {{-1.f, -1.f, -1.f}, {1, 0, 1}},
        {{-1.f, 1.f, -1.f}, {1, 1, 1}},
};

unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,
        0, 4, 5,
        0, 5, 1,
        4, 0, 7,
        7, 0, 3,
        3, 2, 7,
        7, 2, 6,
        1, 5, 2,
        2, 5, 6,
        5, 4, 6,
        6, 4, 7,
};

const glm::ivec2 window_size = {800, 600};
const double fps = 60;
double fps_rate = 1. / fps;
const double upd = 120;
double upd_rate = 1. / upd;

int main() {
    if (!glfwInit()) {
        std::cout << "failed to init glfw\n";
        std::abort();
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    auto* window = glfwCreateWindow(
            window_size.x, window_size.y, "OpenGL example",
            nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "failed to create window\n";
        std::abort();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cout << "failed to initialize glad\n";
        std::abort();
    }

    unsigned int program;

    { // make program
        unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
        glCompileShader(vertex_shader);

        int success;
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
            std::cout << "failed to compile vertex shader\n";
            std::cout << '\n' << info_log << '\n';
            std::abort();
        }

        unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
        glCompileShader(fragment_shader);

        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
            std::cout << "failed to compile fragment shader\n";
            std::cout << '\n' << info_log << '\n';
            std::abort();
        }

        program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);

        glLinkProgram(program);

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char info_log[512];
            glGetProgramInfoLog(program, 512, nullptr, info_log);
            std::cout << "failed to link program\n";
            std::cout << '\n' << info_log << '\n';
            std::abort();
        }

        glDetachShader(program, vertex_shader);
        glDetachShader(program, fragment_shader);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }

    unsigned int vertex_buffer;
    glCreateBuffers(1, &vertex_buffer);
    glNamedBufferData(vertex_buffer, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int indices_buffer;
    glCreateBuffers(1, &indices_buffer);
    glNamedBufferData(indices_buffer, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int vertex_array;
    glCreateVertexArrays(1, &vertex_array);

    glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(Vertex));
    glEnableVertexArrayAttrib(vertex_array, 0);
    glVertexArrayAttribFormat(vertex_array, 0, 3, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(vertex_array, 0, 0);

    glVertexArrayVertexBuffer(vertex_array, 1, vertex_buffer, sizeof(glm::vec3), sizeof(Vertex));
    glEnableVertexArrayAttrib(vertex_array, 1);
    glVertexArrayAttribFormat(vertex_array, 1, 3, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(vertex_array, 1, 1);

    glVertexArrayElementBuffer(vertex_array, indices_buffer);

    glUseProgram(program);
    glBindVertexArray(vertex_array);

    glClearColor(0.f, 0.f, 0.f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);

    int mvp_loc = glGetUniformLocation(program, "mvp");

    auto model = Model();
    auto camera = Camera();
    auto projection = glm::perspective(
            glm::radians(45.f),
            static_cast<float>(window_size.x) / static_cast<float>(window_size.y),
            0.1f, 100.f);

    double last_time, curr_time = glfwGetTime(),
            upd_count_time = 0,
            fps_count_time = 0;

    bool should_redraw = false;

    while (!glfwWindowShouldClose(window)) {
        last_time = curr_time;
        curr_time = glfwGetTime();
        auto duration = curr_time - last_time;
        upd_count_time += duration;
        fps_count_time += duration;
        while (upd_count_time >= upd_rate) {
            upd_count_time -= upd_rate;
            auto time = static_cast<float>(glfwGetTime());
            camera.set_position(glm::vec3(std::sin(time), std::sin(time), std::cos(time)) * glm::vec3(5, 5, 5));
            camera.look_at({0, 0, 0});
            if (!should_redraw) should_redraw = true;
        }
        if (should_redraw && fps_count_time >= fps_rate) {
            glfwPollEvents();
            auto mvp = projection * camera.get_view() * model.get_model();
            glUniformMatrix4fv(mvp_loc, 1, false, glm::value_ptr(mvp));
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
            glfwSwapBuffers(window);
            should_redraw = false;
            fps_count_time -= fps_rate;
        }
    }

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &indices_buffer);

    glfwTerminate();
    return 0;
}