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

class SpotLight
{
private:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;

public:
    SpotLight(glm::vec3 position, glm::vec3 direction);
    void setLightColor(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks);
    void setLightIntensity(float constant, float linear, float quadratic);
    void setSpotArea(float cutOff, float outerCutOff);

    glm::vec3& getPos();
    glm::vec3& getDir();
    glm::vec3& getAmbient();
    glm::vec3& getDiffuse();
    glm::vec3& getSpecular();
    float& getLightIntensityC();
    float& getLightIntensityL();
    float& getLightIntensityQ();
    float& getLightcutOff();
    float& getLightcutOffOuter();

    ~SpotLight();
};
