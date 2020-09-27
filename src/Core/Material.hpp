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
#include "Include/filesystem.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "UniformObjectBuffer.hpp"
#include "Algorithm/NoiseGen.hpp"

class Material
{
protected:
    Shader* shader;
    std::string type;
    std::string filePath;

    glm::vec3 specular;    
    float shininess;

    std::vector<float>* generatedData;

    virtual void initBuffer() = 0;

public:
    Material(Shader* shader);
    virtual void loadTexture(std::string filePath, int colorMode);
    virtual void generateTexture(int width, int height);

    virtual void setMaterialProps(glm::vec3 specular, float shininess);
    void setModelMatrix(glm::mat4& model, glm::mat4& normal);
    void setCameraPos(glm::vec3& pos);
    virtual void draw() = 0;
    virtual ~Material();
};
