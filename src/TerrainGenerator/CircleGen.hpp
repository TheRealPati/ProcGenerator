#pragma once

#include <vector>
#include <cstdio>
#include <time.h>
#include "Utils.hpp"
#include "TerrainGenAlgo.hpp"

class CircleGen : public TerrainGenAlgo
{
private:
public:
    CircleGen(int surfaceSideSize, int density, Randomizer& randomizer);
    ~CircleGen();
    void modifyTerrain(std::vector<VertexPNT>& vertices);
};