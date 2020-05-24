#pragma once

#include <vector>
#include <iostream>
#include "Utils.hpp"
#include "DataStructures/QuadTree.hpp"

class ObjectScatterer
{
private:
    Randomizer& randomizer;
    QuadTree* qtree;
    
public:
    ObjectScatterer(Randomizer& randomizer);
    void feedSurfaceData(std::vector<VertexPNT> surfaceData, float maxSideSize);
    float getHeight(float x, float z);
    ~ObjectScatterer();

};


