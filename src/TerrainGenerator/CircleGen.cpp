#include "CircleGen.hpp"

CircleGen::CircleGen(int surfaceSideSize, int density, Randomizer& randomizer)
: TerrainGenAlgo(surfaceSideSize, density, randomizer)
{}

CircleGen::~CircleGen(){}

void CircleGen::modifyTerrain(std::vector<VertexPNT>& vertices)
{ 
    float displacement = 1.0f;
    int iterationLevel = 1000;
    float circleSize = 8.0f;

    for(int i = 0; i<iterationLevel; i++){
        float x1 = this->rand() * surfaceSideSize/density - surfaceSideSize/density/2;
        float z1 = this->rand() * surfaceSideSize/density - surfaceSideSize/density/2;

        for (auto& vertex: vertices){
            float distance = sqrtf((x1-vertex.position.x)*(x1-vertex.position.x) + (z1-vertex.position.z)*(z1-vertex.position.z));

		    if (distance < circleSize){
                vertex.position.y += displacement/2 + cos(0.125*distance*3.14f)*displacement/2;
            }
        }
        displacement -= 0.001f;
    }
}