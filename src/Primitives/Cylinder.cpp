#include "Cylinder.hpp"

Cylinder::Cylinder(float radius, float height)
{
    this->radius = radius;
    this->height = height;
}

std::vector<VertexPNT> Cylinder::getVertexData()
{
    std::vector<VertexPNT> cylinderData = {};
    const float PI = 3.1415926f;
    for(int i = 0; i < 16; i++)
    {
        VertexPNT pnt;
        pnt.position = glm::vec3(radius * sin(2.0f * PI * i / 16.0f), height * 1.0f, radius * cos(2.0f * PI * i / 16.0f));
        pnt.normal = glm::normalize(pnt.position);
        pnt.texCoords = glm::vec2(i/15.0f, 1.0f);
        cylinderData.emplace_back(pnt);
    }
    for(int i = 0; i < 16; i++)
    {
        VertexPNT pnt;
        pnt.position = glm::vec3(radius * sin(2.0f * PI * i / 16.0f), height * -1.0f, radius * cos(2.0f * PI * i / 16.0f));
        pnt.normal = glm::normalize(pnt.position);
        pnt.texCoords = glm::vec2(i/15.0f, 0.0f);
        cylinderData.emplace_back(pnt);
    }

    return cylinderData;
}

std::vector<GLuint> Cylinder::getIndexData()
{
    std::vector<GLuint> indexData = {};
    for(int i = 0; i < 16; i++){
        indexData.emplace_back(i);
        indexData.emplace_back((i+1)%16);
        indexData.emplace_back(i+16);
        indexData.emplace_back((i+1)%16);
        indexData.emplace_back(i+16);
        indexData.emplace_back((i+1)%16+16);
    }
    return indexData;
}

Cylinder::~Cylinder()
{

}


