#include "ObjectScatterer.hpp"

ObjectScatterer::ObjectScatterer(Randomizer& randomizer) 
: randomizer(randomizer)
{}

void ObjectScatterer::feedSurfaceData(std::vector<VertexPNT> surfaceData, float maxSideSize)
{
    qtree = new QuadTree(0, 0, maxSideSize, maxSideSize);
    for(auto vertex : surfaceData)
    {
        qtree->insert(new Point({vertex.position.x, vertex.position.y, vertex.position.z}));
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