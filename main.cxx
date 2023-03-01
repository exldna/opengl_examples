//
// Created by asuka on 01.03.2023.
//

# include <glad/glad.h>
# include <GLFW/glfw3.h>

# include <iostream>

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

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}