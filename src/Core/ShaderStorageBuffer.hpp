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

#include "Shader.hpp"
#include "Objects/PointLight.hpp"
#include "Objects/DirectionalLight.hpp"
#include "Objects/SpotLight.hpp"

class ShaderStorageBuffer
{
private:
    std::string blockName;
    GLuint bindingPoint;
    GLuint SSBO;

public:
    ShaderStorageBuffer(GLuint bindingPoint);
    void preserveMat(int countOfMatrix, int numOfMatrixEach);
    void setModelMat(glm::mat4& matrix, glm::mat4& normalMatrix, int index);
    void setSkinningMat(glm::mat4& matrix, int index);
    
    ~ShaderStorageBuffer();
};
