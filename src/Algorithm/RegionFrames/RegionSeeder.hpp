#pragma once

// Standard Headers
#include <vector>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include "Utils.hpp"
#include "Frame.hpp"
#include "SphereRegion.hpp"
#include "ConeRegion.hpp"
#include "CylinderRegion.hpp"

enum class FrameRegion
{
    SPHERE,
    CYLINDER,
    CONE
};

class RegionSeeder
{
private:
    Randomizer& randomizer;
    int numberOfPoints;

protected:
    unsigned int surfaceSideSize;
    unsigned int density;
    
public:
    RegionSeeder(Randomizer& randomizer, int numberOfPoints);
    std::vector<Point> seed(Point bottomCenter, float width, float height, FrameRegion type);
    ~RegionSeeder();
};

