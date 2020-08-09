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

class NoiseGen 
{
private:

public:
    NoiseGen();
    
    double noise(glm::vec2 vector);
    double smoothNoise(glm::vec2 vector);
    double interpolateNoise(glm::vec2 vector);
    double fractalNoise(glm::vec2 vector);
    double woodValue(glm::vec2 vector);

    ~NoiseGen();
};