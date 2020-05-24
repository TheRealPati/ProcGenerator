#pragma once

// Standard Headers
#include <vector>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include "Utils.hpp"

struct ColonBranch
{
    glm::vec3 pos;
    glm::vec3 direction;
    glm::vec3 original_direction;
    ColonBranch* parent;
    unsigned int leafCount = 0;
};

class SpaceColonization 
{
private:
    float minDistance;
    float maxDistance;
    std::vector<Point>& points;
public:
    SpaceColonization(float minDistance, float maxDistance, std::vector<Point>& points);
    std::vector<ColonBranch*> colonize(glm::vec3 root, glm::vec3 rootNormal);
    void bodyGrow(ColonBranch* root, std::vector<ColonBranch*>& generatedBranches);
    void leafGrow(ColonBranch* root, std::vector<ColonBranch*>& generatedBranches);
    ~SpaceColonization();
};