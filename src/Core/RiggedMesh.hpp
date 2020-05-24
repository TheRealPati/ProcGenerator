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
#include "Mesh.hpp"

class RiggedMesh : public Mesh
{
private:
    void setup();
    std::vector<InstanceInfo> vertexSkinning;
    GLuint instanceVBO;

public:
    RiggedMesh(std::vector<VertexPNT> vertexData, std::vector<GLuint> indices, std::vector<InstanceInfo> vertexSkinning);
    virtual void draw(int count);
    ~RiggedMesh();
};