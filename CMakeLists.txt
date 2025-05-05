cmake_minimum_required(VERSION 3.10)
project(renderer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenGL_GL_PREFERENCE GLVND)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Find the required packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

message(STATUS "OpenGL libraries: ${OPENGL_LIBRARIES}")
message(STATUS "GLEW libraries: ${GLEW_LIBRARIES}")
message(STATUS "GLFW libraries: ${glfw3_LIBRARIES}")


file(GLOB SOURCES "src/*.cpp")
add_executable(renderer ${SOURCES})
# Add executable
#
# add_executable(${PROJECT_NAME}
#    src/main.cpp
#    src/shaderClass.cpp
#    src/EBO.cpp
#    src/VAO.cpp
#    src/VBO.cpp
#)

target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    GLEW::GLEW
    glfw
)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/shaders)

file(GLOB SHADER_FILES
    "${CMAKE_SOURCE_DIR}/shaders/*.glsl"
    "${CMAKE_SOURCE_DIR}/shaders/*.vert"
    "${CMAKE_SOURCE_DIR}/shaders/*.frag")
if(SHADER_FILES)
    file(COPY ${SHADER_FILES} DESTINATION "${CMAKE_BINARY_DIR}/shaders")
endif()

# Command to copy shaders when they change
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/shaders
    COMMENT "Copying shaders to build directory"
)

# Optional: Print all include directories
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message(STATUS "Include directories:")
foreach(dir ${dirs})
    message(STATUS "  ${dir}")
endforeach()

#Yes, I generated this file using Claude. Judge me for that.
