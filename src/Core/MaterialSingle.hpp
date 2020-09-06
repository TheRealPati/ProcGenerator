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
#include "Algorithm/NoiseGen.hpp"

class MaterialSingle : public Material
{
private:
    std::string type;
    std::string filePath;

    GLuint diffuse;
    glm::vec3 specular;    
    float shininess;

    std::vector<float>* generatedData;

    virtual void initBuffer();

public:
    MaterialSingle(Shader* shader);
    virtual void loadTexture(std::string filePath, int colorMode);
    virtual void generateTexture(int width, int height);
    virtual void setMaterialProps(glm::vec3 specular, float shininess);
    virtual void draw();
    ~MaterialSingle();
};
