#include "CrossedPlane.hpp"

CrossedPlane::CrossedPlane(float scale)
{
    this->scale = scale;
}

std::vector<VertexPNT> CrossedPlane::getVertexData()
{
    std::vector<VertexPNT> planeData = {};
    VertexPNT pnt;
    //0
    pnt.position = glm::vec3(0.0f, 0.0f, 0.0f) * scale;
    pnt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    pnt.texCoords = glm::vec2(0.5f, 0.0f);
    planeData.emplace_back(pnt);
    //1
    pnt.position = glm::vec3(0.0f, 1.0f, 0.0f) * scale;
    pnt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    pnt.texCoords = glm::vec2(0.5f, 1.0f);
    planeData.emplace_back(pnt);

    //Kereszt1
    //2
    pnt.position = glm::vec3(-0.5f, 0.0f, -0.5f) * scale;
    pnt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    pnt.texCoords = glm::vec2(0.0f, 0.0f);
    planeData.emplace_back(pnt);
    //3
    pnt.position = glm::vec3(-0.5f, 1.0f, -0.5f) * scale;
    pnt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    pnt.texCoords = glm::vec2(0.0f, 1.0f);
    planeData.emplace_back(pnt);
    //4
    pnt.position = glm::vec3(0.5f, 0.0f, 0.5f) * scale;
    pnt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    pnt.texCoords = glm::vec2(1.0f, 0.0f);
    planeData.emplace_back(pnt);
    //5
    pnt.position = glm::vec3(0.5f, 1.0f, 0.5f) * scale;
    pnt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    pnt.texCoords = glm::vec2(1.0f, 1.0f);
    planeData.emplace_back(pnt);

    //Kereszt2
    //6
    pnt.position = glm::vec3(-0.5f, 0.0f, 0.5f) * scale;
    pnt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    pnt.texCoords = glm::vec2(0.0f, 0.0f);
    planeData.emplace_back(pnt);
    //7
    pnt.position = glm::vec3(-0.5f, 1.0f, 0.5f) * scale;
    pnt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    pnt.texCoords = glm::vec2(0.0f, 1.0f);
    planeData.emplace_back(pnt);
    //8
    pnt.position = glm::vec3(0.5f, 0.0f, -0.5f) * scale;
    pnt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    pnt.texCoords = glm::vec2(1.0f, 0.0f);
    planeData.emplace_back(pnt);
    //9
    pnt.position = glm::vec3(0.5f, 1.0f, -0.5f) * scale;
    pnt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    pnt.texCoords = glm::vec2(1.0f, 1.0f);
    planeData.emplace_back(pnt);

    //Egyenes
    //10
    pnt.position = glm::vec3(0.8f, 0.0f, 0.0f) * scale;
    pnt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    pnt.texCoords = glm::vec2(0.0f, 0.0f);
    planeData.emplace_back(pnt);
    //11
    pnt.position = glm::vec3(0.8f, 1.0f, 0.0f) * scale;
    pnt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    pnt.texCoords = glm::vec2(0.0f, 1.0f);
    planeData.emplace_back(pnt);
    //12
    pnt.position = glm::vec3(-0.8f, 0.0f, 0.0f) * scale;
    pnt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    pnt.texCoords = glm::vec2(1.0f, 0.0f);
    planeData.emplace_back(pnt);
    //13
    pnt.position = glm::vec3(-0.8f, 1.0f, 0.0f) * scale;
    pnt.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    pnt.texCoords = glm::vec2(1.0f, 1.0f);
    planeData.emplace_back(pnt);


    return planeData;
}

std::vector<GLuint> CrossedPlane::getIndexData()
{
    return {
        0,1,2,
        1,2,3,
        0,1,4,
        1,4,5,
        0,1,6,
        1,6,7,
        0,1,8,
        1,8,9,
        0,1,10,
        1,10,11,
        0,1,12,
        1,12,13
    };
}

CrossedPlane::~CrossedPlane()
{

}


