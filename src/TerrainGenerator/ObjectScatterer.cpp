#include "ObjectScatterer.hpp"

ObjectScatterer::ObjectScatterer(Randomizer& randomizer) 
: randomizer(randomizer)
{}

void ObjectScatterer::feedSurfaceData(std::vector<VertexPNT> surfaceData, int surfaceSideSize, int density)
{
    float corner = surfaceSideSize/density;
    qtree = new QuadTree(0, 0, corner, corner);
    for(auto vertex : surfaceData)
    {
        qtree->insert(new QuadNode({vertex.position.x, vertex.position.y, vertex.position.z}));
    }
    fprintf(stderr, "[INFO] Feeding completed!\n");
}

float ObjectScatterer::getHeight(float x, float z)
{
    return qtree->search(x,z)->y;
}

ObjectScatterer::~ObjectScatterer()
{

}