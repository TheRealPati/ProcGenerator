#pragma once

#include <vector>
#include <iostream>
#include "Utils.hpp"

class TerrainGenAlgo
{
private:
    Randomizer& randomizer;

protected:
    unsigned int surfaceSideSize;
    unsigned int density;

    
public:
    TerrainGenAlgo(int surfaceSideSize, int density, Randomizer& rand);
    virtual void modifyTerrain(std::vector<VertexPNT>& vertices);
    ~TerrainGenAlgo();

    std::vector<VertexPNT> generateVertices();
    std::vector<GLuint> generateIndices();
    void genNormals(std::vector<VertexPNT>& vertices);

    double rand();
    double randCenterZero();
};


