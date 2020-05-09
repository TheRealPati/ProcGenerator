cmake_minimum_required(VERSION 3.7)
project(ProcGenOnlab)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH "cmake")

cmake_policy(SET CMP0072 NEW)
if(CMAKE_HOST_SYSTEM MATCHES "Linux")
    SET( OpenGL_GL_PREFERENCE "GLVND" )
endif()

# OpenGL
find_package(OpenGL REQUIRED)
message(STATUS "OpenGL included at ${OPENGL_INCLUDE_DIR}")

# GLM
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

# GLFW
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")

# GLEW
find_package(GLEW REQUIRED)
message(STATUS "Found GLEW in ${GLEW_INCLUDE_DIR}")

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
endif()

configure_file(config/filesystem.hpp.in src/Include/filesystem.hpp)
include_directories("${CMAKE_SOURCE_DIR}/src")

file(GLOB PROJECT_HEADERS src/Include/*.hpp src/Core/*.hpp src/Objects/*.hpp src/TerrainGenerator/*.hpp src/*.hpp)
file(GLOB PROJECT_SOURCES src/Include/*.cpp src/Core/*.cpp src/Objects/*.cpp src/TerrainGenerator/*.cpp src/*.cpp)
file(GLOB PROJECT_SHADERS src/shader/*.comp
                          src/shader/*.frag
                          src/shader/*.geom
                          src/shader/*.vert)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS})
target_link_libraries(${PROJECT_NAME} ${LIBS} GL GLU GLEW glfw assimp)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
