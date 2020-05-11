#include "QuadTree.hpp"


QuadTree::QuadTree(float topLeftX, float topLeftZ, float botRightX, float botRightZ) 
{ 
    node = NULL;
    topLeftTree  = NULL;
    topRightTree = NULL;
    botLeftTree  = NULL;
    botRightTree = NULL;
    this->topLeftX = topLeftX; 
    this->topLeftZ = topLeftZ; 
    this->botRightX = botRightX; 
    this->botRightZ = botRightZ; 
} 

void QuadTree::divide()
{
    topLeftTree = new QuadTree(topLeftX, topLeftZ, (topLeftX + botRightX)/2.0f, (topLeftZ + botRightZ)/2.0f);
    botLeftTree = new QuadTree(topLeftX, (topLeftZ + botRightZ)/2.0f, (topLeftX + botRightX)/2.0f, botRightZ);
    topRightTree = new QuadTree((topLeftX + botRightX)/2.0f, topLeftZ, botRightX, (topLeftZ + botRightZ)/2.0f);
    botRightTree = new QuadTree((topLeftX + botRightX)/2.0f, (topLeftZ + botRightZ)/2.0f, botRightX, botRightZ);
    divided = true;
    topLeftTree->insert(this->node); 
    botLeftTree->insert(this->node); 
    topRightTree->insert(this->node); 
    botRightTree->insert(this->node); 
}

void QuadTree::insert(QuadNode *node) 
{ 
    if (node == NULL)
        return;

    if (!inBoundary(node->x, node->z)) 
        return;

    if(!hasNode)
    {
        this->node = node;
        hasNode = true;
    }
    else
    {
        if(!divided)
            divide();
        topLeftTree->insert(node); 
        botLeftTree->insert(node); 
        topRightTree->insert(node); 
        botRightTree->insert(node); 
    }
} 

/* void QuadTree::queryRange( std::vector<QuadNode*> found)
{
    //Ha intersect és topLeft nem null
    {
        //Query
    }
    else if(intersect)
    {

    }
}

bool QuadTree::tryPlace(float x, float z, float radius)
{
    if(intersect(x, z, radius))
    {

    }
    return false;
} */

QuadNode* QuadTree::search(float x, float z) 
{ 
    if(hasNode && !divided)
    {
        fprintf(stderr, "Y: %f\n", node->y);
        return node;
    }
    else
    {
        if ((topLeftX + botRightX) / 2 >= x) 
        { 
            if ((topLeftZ + botRightZ) / 2 >= z) 
            {
                return topLeftTree->search(x,z);
            }
            else
            {
                return botLeftTree->search(x,z);
            }
        }
        else
        {
            if((topLeftZ + botRightZ) / 2 >= z)
            {
                return topRightTree->search(x,z);
            }
            else
            {
                return botRightTree->search(x,z);
            }
        }

    }
}

bool QuadTree::inBoundary(float x, float z) 
{ 
    return (x >= topLeftX && 
        x < botRightX && 
        z >= topLeftZ && 
        z < botRightZ); 
}