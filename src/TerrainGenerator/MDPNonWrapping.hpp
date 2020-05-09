#pragma once

#include <vector>
#include <cstdio>
#include <time.h>
#include "Utils.hpp"
#include "TerrainGenAlgo.hpp"

class MDPNonWrapping : public TerrainGenAlgo
{
private:
    void step(std::vector<std::vector<VertexPNT>>& vertices, int currentLength, int min_x, int max_x, int min_z, int max_z, float disp);
public:
    MDPNonWrapping(int surfaceSideSize, int density);
    ~MDPNonWrapping();
    void modifyTerrain(std::vector<VertexPNT>& vertices);
};