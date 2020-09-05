#include "Plane.hpp"

Plane::Plane(float scale)
{
    this->scale = scale;
}

std::vector<VertexPNT> Plane::getVertexData()
{
    std::vector<VertexPNT> planeData = {};
    VertexPNT pnt;
    //0
    pnt.position = glm::vec3(-0.5f, 0.0f, 0.0f) * scale;
    pnt.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    pnt.texCoords = glm::vec2(0.0f, 0.0f);
    planeData.emplace_back(pnt);
    //1
    pnt.position = glm::vec3(0.5f, 0.0f, 0.0f) * scale;
    pnt.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    pnt.texCoords = glm::vec2(1.0f, 0.0f);
    planeData.emplace_back(pnt);
    //2
    pnt.position = glm::vec3(-0.5f, 1.0f, 0.0f) * scale;
    pnt.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    pnt.texCoords = glm::vec2(0.0f, 1.0f);
    planeData.emplace_back(pnt);
    //3
    pnt.position = glm::vec3(0.5f, 1.0f, 0.0f) * scale;
    pnt.normal = glm::vec3(0.0f, 0.0f, 1.0f);
    pnt.texCoords = glm::vec2(1.0f, 1.0f);
    planeData.emplace_back(pnt);


    return planeData;
}

std::vector<GLuint> Plane::getIndexData()
{
    return {
        0,1,2,
        1,2,3
    };
}

Plane::~Plane()
{

}


