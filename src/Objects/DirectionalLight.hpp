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

class DirectionalLight
{
private:
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

public:
    DirectionalLight(glm::vec3 direction);
    void setLightColor(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks);

    glm::vec3& getDir();
    glm::vec3& getAmbient();
    glm::vec3& getDiffuse();
    glm::vec3& getSpecular();

    ~DirectionalLight();
};
