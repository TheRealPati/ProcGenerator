#include "Cube.hpp"

Cube::Cube(float scale)
{
    this->scale = scale;
}

std::vector<VertexPNT> Cube::getVertexData()
{
    std::vector<VertexPNT> cubeData = {};
    VertexPNT pnt;
    //0
    pnt.position = glm::vec3(-1.0f, 1.0f, 1.0f) * scale;
    pnt.normal = pnt.position;
    pnt.texCoords = glm::vec2(1.0f, 1.0f);
    cubeData.emplace_back(pnt);
    //1
    pnt.position = glm::vec3(-1.0f, 1.0f, -1.0f ) * scale;
    pnt.normal = pnt.position;
    pnt.texCoords = glm::vec2(-1.0f, -1.0f);
    cubeData.emplace_back(pnt);
    //2
    pnt.position = glm::vec3(1.0f, 1.0f, 1.0f ) * scale;
    pnt.normal = pnt.position;
    pnt.texCoords = glm::vec2(1.0f, -1.0f);
    cubeData.emplace_back(pnt);
    //3
    pnt.position = glm::vec3(1.0f, 1.0f, -1.0f ) * scale;
    pnt.normal = pnt.position;
    pnt.texCoords = glm::vec2(-1.0f, 1.0f);
    cubeData.emplace_back(pnt);
    //4
    pnt.position = glm::vec3(-1.0f, -1.0f, 1.0f) * scale;
    pnt.normal = pnt.position;
    pnt.texCoords = glm::vec2(-1.0f, 1.0f);
    cubeData.emplace_back(pnt);
    //5
    pnt.position = glm::vec3(-1.0f, -1.0f, -1.0f) * scale;
    pnt.normal = pnt.position;
    pnt.texCoords = glm::vec2(1.0f, -1.0f);
    cubeData.emplace_back(pnt);
    //6
    pnt.position = glm::vec3(1.0f, -1.0f, 1.0f) * scale;
    pnt.normal = pnt.position;
    pnt.texCoords = glm::vec2(-1.0f, -1.0f);
    cubeData.emplace_back(pnt);
    //7
    pnt.position = glm::vec3(1.0f, -1.0f, -1.0f) * scale;
    pnt.normal = pnt.position;
    pnt.texCoords = glm::vec2(1.0f, 1.0f);
    cubeData.emplace_back(pnt);

    return cubeData;
}

std::vector<GLuint> Cube::getIndexData()
{
    return {
        1,3,2,
        1,0,2,
        1,0,4,
        1,5,4,
        0,4,6,
        0,2,6,
        2,3,7,
        2,6,7,
        3,1,5,
        3,7,5,
        4,5,7,
        4,6,7
    };
}

Cube::~Cube()
{

}


