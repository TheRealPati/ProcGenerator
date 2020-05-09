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

// App Headers

class PointLight
{
private:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;

public:
    PointLight(glm::vec3 position);
    void setLightColor(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks);
    void setLightIntensity(float constant, float linear, float quadratic);
    glm::vec3& getPos();
    glm::vec3& getAmbient();
    glm::vec3& getDiffuse();
    glm::vec3& getSpecular();
    float& getLightIntensityC();
    float& getLightIntensityL();
    float& getLightIntensityQ();

    void bindUniform(GLuint shaderProgram);
    ~PointLight();
};
