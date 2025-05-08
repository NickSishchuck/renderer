cmake_minimum_required(VERSION 3.10)
project(renderer)

set(CMAKE_CXX_STANDARD 17)  # Increased to C++17 for std::filesystem
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OpenGL_GL_PREFERENCE GLVND)

# Check if ImGui exists, if not print an error
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/include/imgui/imgui.cpp")
    message(FATAL_ERROR "ImGui not found.")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

# Create logs directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/logs)

# Find the required packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

message(STATUS "OpenGL libraries: ${OPENGL_LIBRARIES}")
message(STATUS "GLEW libraries: ${GLEW_LIBRARIES}")
message(STATUS "GLFW libraries: ${glfw3_LIBRARIES}")

# Add ImGui source files
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/include/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Include ImGui directories
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

file(GLOB SOURCES "src/*.cpp")
add_executable(renderer ${SOURCES} ${IMGUI_SOURCES})

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

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/textures ${CMAKE_BINARY_DIR}/textures
    COMMENT "Copying textures to build directory"
)

# Optional: Print all include directories
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message(STATUS "Include directories:")
foreach(dir ${dirs})
    message(STATUS "  ${dir}")
endforeach()

# Create a log file for CMake output
execute_process(
    COMMAND ${CMAKE_COMMAND} -E echo "CMake configuration completed at ${DATE}"
    OUTPUT_FILE "${CMAKE_BINARY_DIR}/logs/cmake_config.log"
)
