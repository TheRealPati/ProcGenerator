#include "CircleGen.hpp"

CircleGen::CircleGen(int surfaceSideSize, int density)
: TerrainGenAlgo(surfaceSideSize, density)
{}

CircleGen::~CircleGen(){}

void CircleGen::modifyTerrain(std::vector<VertexPNT>& vertices)
{ 
    float displacement = 1.0f;
    int iterationLevel = 1000;
    //int iterationLevel = 5;
    float circleSize = 8.0f;

    for(int i = 0; i<iterationLevel; i++){
        float x1 = this->rand() * surfaceSideSize/density - surfaceSideSize/density/2;
        float z1 = this->rand() * surfaceSideSize/density - surfaceSideSize/density/2;

        for (auto& vertex: vertices){
            float distance = sqrtf((x1-vertex.position.x)*(x1-vertex.position.x) + (z1-vertex.position.z)*(z1-vertex.position.z));

		    if (distance < circleSize){
                vertex.position.y += displacement/2 + cos(0.125*distance*3.14f)*displacement/2;
            }
		    /*else{
                vertex.position.y -= displacement;
                vertex.color.b -= displacement*0.5f;
                vertex.color.g += displacement*0.3f;
                vertex.color.r += displacement*0.3f;
            }*/
        }
        //displacement *= 0.98f;
        displacement -= 0.001f;
    }
}