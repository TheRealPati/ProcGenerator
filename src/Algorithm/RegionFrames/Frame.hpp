#pragma once

// Standard Headers
#include <vector>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include "Utils.hpp"

class Frame
{
protected:
    float width;
    float height;
public:
    Frame(float width, float height) : width(width), height(height){};
    virtual bool inBound(Point point) = 0;
    ~Frame(){};
};
