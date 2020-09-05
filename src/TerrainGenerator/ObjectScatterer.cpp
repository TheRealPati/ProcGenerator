#include "ObjectScatterer.hpp"

ObjectScatterer::ObjectScatterer(Randomizer& randomizer) 
: randomizer(randomizer)
{}

void ObjectScatterer::feedSurfaceData(std::vector<VertexPNT> surfaceData, float maxSideSize)
{
    qtree = new QuadTree(0, 0, maxSideSize, maxSideSize);
    for(auto vertex : surfaceData)
    {
        PN* point = new PN;
        point->position = {vertex.position.x, vertex.position.y, vertex.position.z};
        point->normal = {-vertex.normal.x, -vertex.normal.y, -vertex.normal.z};
        qtree->insert(point);
    }
    fprintf(stderr, "[INFO] Feeding completed!\n");
}

float ObjectScatterer::getHeight(float x, float z)
{
    return qtree->search(x,z)->position.y;
}

glm::vec3 ObjectScatterer::getNormal(float x, float z)
{
    return qtree->search(x,z)->normal;
}

glm::mat4 ObjectScatterer::calcRotMat(const glm::vec3& originalAxis, const glm::vec3& modifiedAxis)
{
    glm::vec3 axis = glm::cross(originalAxis, modifiedAxis);
    float angle = acos(glm::dot(originalAxis, modifiedAxis));
    glm::mat4 rotMat = glm::mat4(1.0f);
    if(angle != 0)
        rotMat = glm::rotate(glm::mat4(1.0f), angle, axis);

    return rotMat;
}

ObjectScatterer::~ObjectScatterer()
{

}