#pragma once
// System Headers
#define GL_GLEXT_PROTOTYPES
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <GL/gl.h>

struct VertexPC {
    glm::vec3 position;
    glm::vec3 color;
};

struct VertexPNC {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
};

struct VertexP {
    glm::vec3 position;
};

struct VertexPNT {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};