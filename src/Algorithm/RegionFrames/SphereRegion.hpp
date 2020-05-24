#pragma once

// Standard Headers
#include <vector>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include "Utils.hpp"
#include "Frame.hpp"

class SphereRegion : public Frame
{
public:
    SphereRegion(float width, float height) : Frame(width, height){};
    virtual bool inBound(Point point)
    {
        return point.x * point.x + (point.y - height/2) * (point.y - height/2) + point.z * point.z < width * width;
    };
    ~SphereRegion(){};
};