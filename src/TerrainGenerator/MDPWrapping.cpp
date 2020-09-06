#include "MDPWrapping.hpp"

MDPWrapping::MDPWrapping(int surfaceSideSize, int density, Randomizer& randomizer)
: TerrainGenAlgo(surfaceSideSize, density, randomizer)
{
    setSmoothFilter(1);
}

MDPWrapping::~MDPWrapping(){}

void MDPWrapping::centerStep(std::vector<std::vector<VertexPNT>>&vertices, std::vector<int>& corners, float disp) {
    for(unsigned int i = 0; i < corners.size(); i+=4)
    {
        int min_x = corners[i];
        int max_x = corners[i+1];
        int min_z = corners[i+2];
        int max_z = corners[i+3];
        int avgOfX = (max_x+min_x)/2;
        int avgOfZ = (max_z+min_z)/2;

        vertices[avgOfX][avgOfZ].position.y = (
            vertices[min_x][min_z].position.y +
            vertices[min_x][max_z].position.y +
            vertices[max_x][min_z].position.y +
            vertices[max_x][max_z].position.y
        )/4 + this->rand() * disp;
    }
}

std::vector<int> MDPWrapping::sideStep(std::vector<std::vector<VertexPNT>>& vertices, int& currentLength, std::vector<int>& corners, float disp) {

    std::vector<int> newCorners = {};
    for(unsigned int i = 0; i < corners.size(); i+=4)
    {
        int min_x = corners[i];
        int max_x = corners[i+1];
        int min_z = corners[i+2];
        int max_z = corners[i+3];
        int avgOfX = (max_x+min_x)/2;
        int avgOfZ = (max_z+min_z)/2;

        //Right side
        float rightPoint = 
            vertices[min_x][max_z].position.y +
            vertices[max_x][max_z].position.y +
            vertices[avgOfX][avgOfZ].position.y;
        if(avgOfX + 2 * currentLength <= surfaceSideSize-1)
        {
            rightPoint += vertices[avgOfX + 2 * currentLength][avgOfZ].position.y;
            rightPoint /= 4;
        }
        else
        {
            rightPoint /= 3;
        }
        //rightPoint += this->rand() * disp;
        vertices[avgOfX+currentLength][avgOfZ].position.y = rightPoint;

        // Left side
        float leftPoint =
            vertices[min_x][min_z].position.y +
            vertices[max_x][min_z].position.y +
            vertices[avgOfX][avgOfZ].position.y;
        if(avgOfX - 2 * currentLength >= 0)
        {
            leftPoint += vertices[avgOfX - 2 * currentLength][avgOfZ].position.y;
            leftPoint /= 4;
        }
        else
        {
            leftPoint /= 3;
        }
        //leftPoint += this->rand() * disp;
        vertices[avgOfX-currentLength][avgOfZ].position.y = leftPoint;

        // Up side
        float upPoint =
            vertices[min_x][min_z].position.y +
            vertices[min_x][max_z].position.y +
            vertices[avgOfX][avgOfZ].position.y;
        if(avgOfZ - 2 * currentLength >= 0)
        {
            upPoint += vertices[avgOfX][avgOfZ - 2 * currentLength].position.y;
            upPoint /= 4;
        }
        else
        {
            upPoint /= 3;
        }
        //upPoint += this->rand() * disp;
        vertices[avgOfX][avgOfZ+currentLength].position.y = upPoint;

        // Down side
        float downPoint =
            vertices[max_x][min_z].position.y +
            vertices[max_x][max_z].position.y +
            vertices[avgOfX][avgOfZ].position.y;
        if(avgOfZ + 2 * currentLength <= surfaceSideSize-1)
        {
            downPoint += vertices[avgOfX][avgOfZ + 2 * currentLength].position.y;
            downPoint /= 4;
        }
        else
        {
            downPoint /= 3;
        }
        //downPoint += this->rand() * disp;
        vertices[avgOfX][avgOfZ-currentLength].position.y = downPoint;

        //Push the new corners
        newCorners.insert(newCorners.end(), {min_x, avgOfX, min_z, avgOfZ});
        newCorners.insert(newCorners.end(), {avgOfX, max_x, min_z, avgOfZ});
        newCorners.insert(newCorners.end(), {min_x, avgOfX, avgOfZ, max_z});
        newCorners.insert(newCorners.end(), {avgOfX, max_x, avgOfZ, max_z});

    }
    currentLength /= 2;
    return newCorners;
}

void MDPWrapping::modifyTerrain(std::vector<VertexPNT>& vertices)
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
            
            pc.texCoords.x = (float)(i%surfaceSideSize);
            pc.texCoords.y = (float)(j);
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

        // Initial values
        vertices2D[min_x][min_z].position.y = this->rand() * 100.0f;
        vertices2D[min_x][max_z].position.y = this->rand() * 100.0f;
        vertices2D[max_x][min_z].position.y = this->rand() * 100.0f;
        vertices2D[max_x][max_z].position.y = this->rand() * 100.0f;

        std::vector<int> corners = {min_x, max_x, min_z, max_z};

        int currentLength = surfaceSideSize/2;
        float disp = 16.0f;

        //Wrapping version
        while(currentLength >= 1)
        {
            centerStep(vertices2D, corners, disp);
            corners = sideStep(vertices2D, currentLength, corners, disp);
            disp *= 0.96f;
        }
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
