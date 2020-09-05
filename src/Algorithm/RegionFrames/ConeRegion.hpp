#pragma once

// Standard Headers
#include <vector>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include "Utils.hpp"
#include "Frame.hpp"

class ConeRegion : public Frame
{
public:
    ConeRegion(float width, float height) : Frame(width, height){};
    virtual bool inBound(PN point)
    {
        return point.position.x * point.position.x + (point.position.y - height/2) * (point.position.y - height/2) + point.position.z * point.position.z < width * width;
    };
    ~ConeRegion(){};
};