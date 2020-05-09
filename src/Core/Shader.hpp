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

class Shader
{
private:
    GLuint ID;
    
    std::string loadContent(std::string filename);
    GLuint compileShaders(const GLchar* vertexProgram, const GLchar* fragmentProgram);
    void check(GLuint program);

public:
    Shader(std::string& vertexPath, std::string& fragmentPath);
    void use();
    GLuint getID();
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setVec3(const std::string& name, glm::vec3 value);
    void setMatrix(const std::string& name, glm::mat4 value);
    ~Shader();
};
