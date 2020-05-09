#pragma once

#include <vector>
#include "Utils.hpp"
#include "TerrainGenAlgo.hpp"

class PlainGen : public TerrainGenAlgo
{
private:
public:
    PlainGen(int surfaceSideSize, int density);
    ~PlainGen();
    void modifyTerrain(std::vector<VertexPNT>& vertices);
};