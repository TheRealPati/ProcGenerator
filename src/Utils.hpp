#pragma once
// System Headers
#define GL_GLEXT_PROTOTYPES
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <GL/gl.h>

#include <random>

struct VertexPC 
{
    glm::vec3 position;
    glm::vec3 color;
};

struct VertexPNC 
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
};

struct VertexP 
{
    glm::vec3 position;
};

struct VertexPNT 
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct QuadNode 
{ 
    float x;
    float y;
    float z;
}; 

class Randomizer
{
private:
    std::random_device rd;
    std::mt19937 mt;
    std::uniform_real_distribution<float> dist;

public:
    Randomizer() : rd{}, mt{rd()}, dist{0.0, 1.0}{}
    ~Randomizer(){};
    double rand() 
    {
        return this->dist(mt);
    }

    double randCenterZero() 
    {
        return this->dist(mt) - 0.5f;
    }
};