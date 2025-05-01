cmake_minimum_required(VERSION 3.10)
project(renderer)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set OpenGL preference to avoid warning
set(OpenGL_GL_PREFERENCE GLVND)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find the required packages
find_package(OpenGL REQUIRED)

# For some reason there's an error. It doesn't affect the build.
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# Print status information (useful for debugging)
message(STATUS "OpenGL libraries: ${OPENGL_LIBRARIES}")
message(STATUS "GLEW libraries: ${GLEW_LIBRARIES}")
message(STATUS "GLFW libraries: ${glfw3_LIBRARIES}")

# Add executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    GLEW::GLEW
    glfw
)

# Optional: Print all include directories
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message(STATUS "Include directories:")
foreach(dir ${dirs})
    message(STATUS "  ${dir}")
endforeach()


# Copy assets if needed
# file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})


#Yes, I generated this file using Claude. Judge me for that.
