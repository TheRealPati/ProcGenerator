#pragma once

#include <vector>
#include <cstdio>
#include <time.h>
#include "Utils.hpp"
#include "TerrainGenAlgo.hpp"

class MDPWrapping : public TerrainGenAlgo
{
private:
    void centerStep(std::vector<std::vector<VertexPNT>>& vertices, std::vector<int>& corners, float disp);
    std::vector<int> sideStep(std::vector<std::vector<VertexPNT>>& vertices, int& currentLength, std::vector<int>& corners, float disp);

    float maxY;
    float minY;

    void boundCheck(float point);

public:
    MDPWrapping(int surfaceSideSize, int density, Randomizer& randomizer);
    ~MDPWrapping();
    void modifyTerrain(std::vector<VertexPNT>& vertices);
};