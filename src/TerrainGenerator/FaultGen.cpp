#include "FaultGen.hpp"

FaultGen::FaultGen(int surfaceSideSize, int density, Randomizer& randomizer)
: TerrainGenAlgo(surfaceSideSize, density, randomizer)
{}

FaultGen::~FaultGen(){}

void FaultGen::modifyTerrain(std::vector<VertexPNT>& vertices)
{
    float displacement = 0.2f;
    int iterationLevel = 100;

    for(int i = 0; i<iterationLevel; i++)
    {
        float x1 = this->rand() * surfaceSideSize/density - surfaceSideSize/density/2;
        float x2 = this->rand() * surfaceSideSize/density - surfaceSideSize/density/2;
        float z1 = this->rand() * surfaceSideSize/density - surfaceSideSize/density/2;
        float z2 = this->rand() * surfaceSideSize/density - surfaceSideSize/density/2;

        for (auto& vertex: vertices)
        {
		    if (((x2 - x1) * (vertex.position.z - z1) - (z2 - z1) * (vertex.position.x - x1)) > 0 )
            {
                vertex.position.y += displacement;
            }
		    else
            {
                vertex.position.y -= displacement;
            }
        }
    }
}