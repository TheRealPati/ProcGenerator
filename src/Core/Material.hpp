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

#include "Include/stb_image.hpp"
#include "Shader.hpp"

class Material
{
protected:
    Shader* shader;

    virtual void initBuffer() = 0;

public:
    Material(Shader* shader) { this->shader = shader; };
    virtual void loadTexture(std::string filePath, int colorMode) = 0;
    virtual void generateTexture(int width, int height) = 0;

    virtual void setMaterialProps(glm::vec3 specular, float shininess) = 0;
    void setModelMatrix(glm::mat4& model, glm::mat4& normal){
        shader->setMatrix("model", model);
        shader->setMatrix("normalMatrix", normal);
    };
    void setCameraPos(glm::vec3& pos){
        shader->setVec3("camera.positon", pos);
    };
    virtual void draw() = 0;
    virtual ~Material() {};
};
