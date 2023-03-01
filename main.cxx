//
// Created by asuka on 01.03.2023.
//

# include <glad/glad.h>
# include <GLFW/glfw3.h>

# include <iostream>
# include <array>

const char* vertex_shader_source =
        "#version 330 core\n"
        "void main() {\n"
        "   gl_Position = vec4(0, 0, 0, 0);\n"
        "}";

const char* fragment_shader_source =
        "#version 330 core\n"
        "out vec4 frag_color;\n"
        "void main() {\n"
        "   frag_color = vec4(0.1, 0.2, 0.8, 1.0);\n"
        "}";

int main() {
    if (!glfwInit()) {
        std::cout << "failed init glfw\n";
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    auto* window = glfwCreateWindow(900, 600, "OpenGL example", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "failed create window\n";
        return 1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL()) {
        std::cout << "failed to initialize glad\n";
        return -1;
    }

    glClearColor(0.8, 0.2, 0.1, 0.0);

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glLinkProgram(program);

    glDetachShader(program, vertex_shader);
    glDetachShader(program, fragment_shader);
    glDeleteShader(vertex_shader);
    vertex_shader = 0;
    glDeleteShader(fragment_shader);
    fragment_shader = 0;

    std::array<float, 9> vertices = {
            -0.5, -0.5, 0.f,
            0.f, 0.5, 0.f,
            0.5, -0.5, 0.f,
    };

    unsigned int vertex_buffer = 0;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}