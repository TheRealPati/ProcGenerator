// Standard Headers
#include <vector>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include "Utils.hpp"

class QuadTree 
{
private:
    float topLeftX; 
    float topLeftZ; 
    float botRightX; 
    float botRightZ; 

    PN* node; 
    bool hasNode = false;
    bool divided = false;

    QuadTree* topLeftTree = nullptr; 
    QuadTree* topRightTree = nullptr; 
    QuadTree* botLeftTree = nullptr; 
    QuadTree* botRightTree = nullptr; 

public:
    QuadTree(float topLeftX, float topLeftZ, float botRightX, float botRightZ);
    ~QuadTree();
    void insert(PN*); 
    void divide();
    PN* search(float x, float z); 
    bool inBoundary(float x, float z); 
};