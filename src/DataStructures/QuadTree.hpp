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

    Point* node; 
    bool hasNode = false;
    bool divided = false;

    QuadTree* topLeftTree; 
    QuadTree* topRightTree; 
    QuadTree* botLeftTree; 
    QuadTree* botRightTree; 

public:
    QuadTree(float topLeftX, float topLeftZ, float botRightX, float botRightZ);
    ~QuadTree();
    void insert(Point*); 
    void divide();
    Point* search(float x, float z); 
    bool inBoundary(float x, float z); 
};