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

protected:
    unsigned int surfaceSideSize;
    unsigned int density;
    
public:
    ObjectScatterer(Randomizer& randomizer);
    void feedSurfaceData(std::vector<VertexPNT> surfaceData, int surfaceSideSize, int density);
    float getHeight(float x, float z);
    ~ObjectScatterer();

};


