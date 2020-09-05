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

#include "Core/Mesh.hpp"
#include "Utils.hpp"
#include "TerrainGenerator/ObjectScatterer.hpp"

class Grassfield
{
private:
    Randomizer& randomizer;
    ObjectScatterer* scatterer;

    const float PI = 3.1415926f;

public:
    Grassfield(Randomizer& randomizer, ObjectScatterer* scatterer);
    void populate(unsigned int maxSideSize, std::vector<glm::mat4>& billboardModelMatrixes, unsigned int grassCount);

    ~Grassfield();
};
