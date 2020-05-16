#pragma once
// System Headers
#define GL_GLEXT_PROTOTYPES
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <GL/gl.h>

// Standard Headers
#include <vector>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>

#include "Utils.hpp"

class Cube
{
public:
    float scale;

    Cube(float scale);
    std::vector<VertexPNT> getVertexData();
    std::vector<GLuint> getIndexData();
    ~Cube();
};
