cmake_minimum_required(VERSION 3.10)
project(renderer)

set(CMAKE_CXX_STANDARD 17)  # Increased to C++17 for std::filesystem
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenGL_GL_PREFERENCE GLVND)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Create logs directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/logs)

# Find the required packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

message(STATUS "OpenGL libraries: ${OPENGL_LIBRARIES}")
message(STATUS "GLEW libraries: ${GLEW_LIBRARIES}")
message(STATUS "GLFW libraries: ${glfw3_LIBRARIES}")

file(GLOB SOURCES "src/*.cpp")
add_executable(renderer ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    GLEW::GLEW
    glfw
)

# Create shaders directory
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/shaders)

# Find and copy shader files
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

# Create a log file for CMake output
execute_process(
    COMMAND ${CMAKE_COMMAND} -E echo "CMake configuration completed at ${date}"
    OUTPUT_FILE "${CMAKE_BINARY_DIR}/logs/cmake_config.log"
)

#Yes, I generated this file using Claude. Judge me for that.
