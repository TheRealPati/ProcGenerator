#pragma once

#include <vector>
#include <cstdio>
#include <time.h>
#include "Utils.hpp"
#include "TerrainGenAlgo.hpp"

class FaultGenSine : public TerrainGenAlgo
{
private:
public:
    FaultGenSine(int surfaceSideSize, int density, Randomizer& randomizer);
    ~FaultGenSine();
    void modifyTerrain(std::vector<VertexPNT>& vertices);
};