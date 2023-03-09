
add_library(settings INTERFACE)
target_compile_features(settings INTERFACE cxx_std_20)

find_package(glm REQUIRED)
target_link_libraries(settings INTERFACE glm::glm)

find_package(glad REQUIRED)
target_link_libraries(settings INTERFACE glad::glad)

find_package(glfw3 REQUIRED)
target_link_libraries(settings INTERFACE glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(settings INTERFACE OpenGL::GL)

add_subdirectory(lib)
add_subdirectory(hello_triangle)
add_subdirectory(space)
add_subdirectory(rectangle)
add_subdirectory(cube)
