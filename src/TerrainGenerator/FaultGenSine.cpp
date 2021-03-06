#include "FaultGenSine.hpp"

FaultGenSine::FaultGenSine(int surfaceSideSize, int density, Randomizer& randomizer)
: TerrainGenAlgo(surfaceSideSize, density, randomizer)
{}

FaultGenSine::~FaultGenSine(){}

void FaultGenSine::modifyTerrain(std::vector<VertexPNT>& vertices)
{
    float displacement = 0.2f;
    int iterationLevel = 100;

    for(int i = 0; i<iterationLevel; i++)
    {
        float v = this->rand();
	    float a = sin(v);
	    float b = cos(v);
	    float diagonal = sqrt(surfaceSideSize + surfaceSideSize);
	    float c = this->rand() * surfaceSideSize/density - surfaceSideSize/density/2;

        for (auto& vertex: vertices){
            float distance = a*vertex.position.x + b*vertex.position.z - c;
		    if (fabs(distance) < 1.56f){
                vertex.position.y += 0.5f*sin(distance);
            }
            else if(distance > 1.56f){
                vertex.position.y += 0.5f;
            }
            else if(distance < -1.56f){
                vertex.position.y -= 0.5f;
            }
        }
    }
}