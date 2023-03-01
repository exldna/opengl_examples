//
// Created by asuka on 01.03.2023.
//

# include <GLFW/glfw3.h>
# include <iostream>

int main() {
    if (!glfwInit()) {
        std::cout << "failed init glfw\n";
        return 1;
    }

    auto* window = glfwCreateWindow(900, 600, "OpenGL example", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "failed create window\n";
        return 1;
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}