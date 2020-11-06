#include "Shader.hpp"

Shader::Shader(std::string& vertexShaderName, std::string& fragmentShaderName)
{
    std::string vertexCode;
    std::string fragmentCode;

    vertexCode = loadContent(vertexShaderName);
    fragmentCode = loadContent(fragmentShaderName);

    const GLchar* vertexProgram = vertexCode.c_str();
    const GLchar* fragmentProgram = fragmentCode.c_str();

    ID = compileShaders(vertexProgram, fragmentProgram);
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

std::string Shader::loadContent(std::string filename)
{
    // Shaders
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint Shader::compileShaders(const GLchar* vertexProgram, const GLchar* fragmentProgram)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexProgram, nullptr);
    glCompileShader(vertexShader);
    check(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentProgram, nullptr);
    glCompileShader(fragmentShader);
    check(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glBindFragDataLocation(shaderProgram, 0, "outColor");

    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void Shader::check(GLuint program)
{
    GLint status;
    glGetShaderiv(program, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE){
        char errorBuffer[1024];
        glGetShaderInfoLog(program, 1024, nullptr, errorBuffer);
        fprintf(stderr, "%s\n", errorBuffer);
    }
}

void Shader::use()
{
    glUseProgram(ID);
}

GLuint Shader::getID()
{
    return ID;
}

void Shader::setBool(const std::string &name, bool value)
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value)
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value)
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setVec3(const std::string& name, glm::vec3 value)
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setMatrix(const std::string &name, glm::mat4 value)
{ 
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}