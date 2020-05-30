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
#include <map>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>

#include "Model.hpp"
#include "Objects/Camera.hpp"

class GameObject
{
private:
    Model* model;
    int instanceCount = 1;

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    glm::mat4 normalMatrix = glm::mat4(1.0f);
public:
    GameObject();
    void setModel(Model* model);
    void setMatrix(glm::mat4 matrix);
    void setInstanceCount(int count);
    void draw(Camera* camera);
    ~GameObject();
};
