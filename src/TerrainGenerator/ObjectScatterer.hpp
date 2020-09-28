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
    glm::vec3 getNormal(float x, float z);
    glm::mat4 calcRotMat(glm::vec3& originalAxis, const glm::vec3& modifiedAxis);
    ~ObjectScatterer();

};


