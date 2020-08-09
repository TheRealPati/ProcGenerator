#include "MDPNonWrapping.hpp"

MDPNonWrapping::MDPNonWrapping(int surfaceSideSize, int density, Randomizer& randomizer)
: TerrainGenAlgo(surfaceSideSize, density, randomizer)
{}

MDPNonWrapping::~MDPNonWrapping(){}

void MDPNonWrapping::step(std::vector<std::vector<VertexPNT>>&vertices, int currentLength, int min_x, int max_x, int min_z, int max_z, float disp) {
    if(currentLength < 1)
        return;

    int avgOfX = (max_x+min_x)/2;
    int avgOfZ = (max_z+min_z)/2;

    // Center point
    vertices[avgOfX][avgOfZ].position.y = (
        vertices[min_x][min_z].position.y +
        vertices[min_x][max_z].position.y +
        vertices[max_x][min_z].position.y +
        vertices[max_x][max_z].position.y
    )/4 + disp;

    // Sidepoints
    // Right side
    vertices[avgOfX+currentLength][avgOfZ].position.y = (
        vertices[min_x][max_z].position.y +
        vertices[max_x][max_z].position.y +
        vertices[avgOfX][avgOfZ].position.y
    )/3 + disp;
    // Left side
    vertices[avgOfX-currentLength][avgOfZ].position.y = (
        vertices[min_x][min_z].position.y +
        vertices[max_x][min_z].position.y +
        vertices[avgOfX][avgOfZ].position.y
    )/3 + disp;
    // Up side
    vertices[avgOfX][avgOfZ+currentLength].position.y = (
        vertices[min_x][min_z].position.y +
        vertices[min_x][max_z].position.y +
        vertices[avgOfX][avgOfZ].position.y
    )/3 + disp;
    // Down side
    vertices[avgOfX][avgOfZ-currentLength].position.y = (
        vertices[max_x][min_z].position.y +
        vertices[max_x][max_z].position.y +
        vertices[avgOfX][avgOfZ].position.y
    )/3 + disp;

    //1. square
    step(vertices, currentLength/2, min_x, avgOfX, min_z, avgOfZ, disp * this->randCenterZero() * 1.8f);
    //2. square
    step(vertices, currentLength/2, min_x+currentLength, max_x, min_z, avgOfZ, disp * this->randCenterZero() * 1.8f);
    //3. square
    step(vertices, currentLength/2, min_x, avgOfX, min_z+currentLength, max_z, disp * this->randCenterZero() * 1.8f);
    //4. square
    step(vertices, currentLength/2, min_x+currentLength, max_x, min_z+currentLength, max_z, disp * this->randCenterZero() * 1.8f);
}

void MDPNonWrapping::modifyTerrain(std::vector<VertexPNT>& vertices)
{
    if((ceil(log2(surfaceSideSize-1)) != floor(log2(surfaceSideSize-1))))
    {
        fprintf(stderr, "[ERROR] Mid Point Displacement algorithm required 2^n+1 point on the side\n");
    }
    else
    {
        std::vector<std::vector<VertexPNT>> vertices2D = {};
        int j = 0;
        std::vector<VertexPNT> row = {};
        for(unsigned int i = 0; i < vertices.size(); i++) {
            VertexPNT pc;
            pc.position.x = (float)(i%surfaceSideSize)/density;
            pc.position.z = (float)j/density;
            pc.position.y = 0;
            pc.texCoords.x = (float)(i%surfaceSideSize)/surfaceSideSize;
            pc.texCoords.y = (float)j/surfaceSideSize;
            row.emplace_back(pc);
            if(i%surfaceSideSize == surfaceSideSize-1)
            {
                vertices2D.emplace_back(row);
                row.clear();
                j++;
            }
        }

        int max_x = surfaceSideSize-1;
        int min_x = 0;
        int max_z = surfaceSideSize-1;
        int min_z = 0;
        float disp = 2.0f;

        // Initial values
        vertices2D[min_x][min_z].position.y = 1.0f;
        vertices2D[min_x][max_z].position.y = 1.0f;
        vertices2D[max_x][min_z].position.y = 1.0f;
        vertices2D[max_x][max_z].position.y = 1.0f;

        int currentLength = surfaceSideSize/2;

        // Non-wrapping version
        step(vertices2D, currentLength, min_x, max_x, min_z, max_z, disp);
        fprintf(stderr, "[INFO] MDP Generating done!\n");

        // Put it in one vector
        vertices.clear();
        for(unsigned int i = 0; i < surfaceSideSize; i++)
        {
            for(unsigned int j = 0; j < surfaceSideSize; j++)
            {
                vertices.emplace_back(vertices2D[i][j]);
            }
        }
    }
}
