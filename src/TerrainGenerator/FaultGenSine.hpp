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
    FaultGenSine(int surfaceSideSize, int density);
    ~FaultGenSine();
    void modifyTerrain(std::vector<VertexPNT>& vertices);
};