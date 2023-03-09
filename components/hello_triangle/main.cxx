//
// Created by asuka on 01.03.2023.
//

# include <glad/glad.h>
# include <GLFW/glfw3.h>

# include <iostream>

const char* vertex_shader_source =
        "#version 330 core\n"
        "layout (location = 0) in vec2 position;\n"
        "void main() {\n"
        "   gl_Position = vec4(position, 0, 1);\n"
        "}";

const char* fragment_shader_source =
        "#version 330 core\n"
        "out vec4 frag_color;\n"
        "void main() {\n"
        "   frag_color = vec4(0, 1, 0, 1);\n"
        "}";

struct Vertex {
    float x;
    float y;
};

Vertex vertices[] = {
        {-0.5f, -0.5f},
        {0.0f,  0.5f},
        {0.5f,  -0.5f},
};

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

    auto* window = glfwCreateWindow(900, 600, "OpenGL example", nullptr, nullptr);
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

    unsigned int vertex_array;
    glCreateVertexArrays(1, &vertex_array);
    // attach buffer
    glVertexArrayVertexBuffer(vertex_array, 0, vertex_buffer, 0, sizeof(Vertex));
    // configure vertex attributes
    glEnableVertexArrayAttrib(vertex_array, 0);
    glVertexArrayAttribFormat(vertex_array, 0, 2, GL_FLOAT, false, 0);
    glVertexArrayAttribBinding(vertex_array, 0, 0);

    glUseProgram(program);
    glBindVertexArray(vertex_array);

    glClearColor(1.f, 1.f, 1.f, 1.f);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
    }

    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteBuffers(1, &vertex_buffer);

    glfwTerminate();
    return 0;
}