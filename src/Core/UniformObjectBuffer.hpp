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

class UniformBufferObject
{
private:
    std::string blockName;
    GLuint bindingPoint;
    GLuint UBO;

public:
    UniformBufferObject(GLuint bindingPoint);
    void preserveVPMatrix();
    void preservePointLight(int countOfLamps);
    void preserveDirectionalLight();
    void preserveSpotLight();
    void setVPmatrix(glm::mat4 view, glm::mat4 projection);
    void setPointLights(PointLight* light, int index);
    void setDirectionLights(DirectionalLight* light);
    void setSpotLights(SpotLight* light);
    
    ~UniformBufferObject();
};
