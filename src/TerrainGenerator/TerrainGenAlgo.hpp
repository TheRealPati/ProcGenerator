#pragma once

#include <vector>
#include <random>
#include <iostream>
#include "Utils.hpp"

class TerrainGenAlgo
{
private:
    std::random_device rd;
    std::mt19937 mt;
    std::uniform_real_distribution<float> dist;

protected:
    unsigned int surfaceSideSize;
    unsigned int density;

    
public:
    TerrainGenAlgo(int surfaceSideSize, int density);
    virtual void modifyTerrain(std::vector<VertexPNT>& vertices);
    ~TerrainGenAlgo();

    std::vector<VertexPNT> generateVertices();
    std::vector<GLuint> generateIndices();
    void genNormals(std::vector<VertexPNT>& vertices);

    double rand();
    double randCenterZero();
};


