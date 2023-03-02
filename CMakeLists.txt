cmake_minimum_required(VERSION 3.14)
project(opengl_example CXX)

add_executable(opengl_example main.cxx)

find_package(glad REQUIRED)
target_link_libraries(opengl_example glad::glad)

find_package(glfw3 REQUIRED)
target_link_libraries(opengl_example glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(opengl_example OpenGL::GL)