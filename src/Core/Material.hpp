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
#include "Algorithm/NoiseGen.hpp"

class Material
{
private:
    Shader* shader;
    std::string type;
    std::string filePath;

    GLuint diffuse;
    glm::vec3 specular;    
    float shininess;

    std::vector<float>* generatedData;

    void initBuffer();

public:
    Material(Shader* shader);
    void loadTexture(std::string filePath);
    void generateTexture(int width, int height);
    std::string getName();
    void setMaterialProps(glm::vec3 specular, float shininess);
    void setModelMatrix(glm::mat4& model, glm::mat4& normal);
    void setCameraPos(glm::vec3& pos);
    void draw();
    ~Material();
};
