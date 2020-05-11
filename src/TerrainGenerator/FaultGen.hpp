#pragma once

#include <vector>
#include <cstdio>
#include <time.h>
#include "Utils.hpp"
#include "TerrainGenAlgo.hpp"

class FaultGen : public TerrainGenAlgo
{
private:
public:
    FaultGen(int surfaceSideSize, int density, Randomizer& randomizer);
    ~FaultGen();
    void modifyTerrain(std::vector<VertexPNT>& vertices);
};