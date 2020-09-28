#pragma once
// System Headers
#define GL_GLEXT_PROTOTYPES
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <GL/gl.h>

// Standard Headers
#include <vector>
#include <map>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>

#include "Core/Mesh.hpp"
#include "Core/Importer.hpp"
#include "Utils.hpp"
#include "TerrainGenerator/ObjectScatterer.hpp"
#include "Algorithm/RegionFrames/RegionSeeder.hpp"
#include "Algorithm/SpaceColonization.hpp"

class Forest
{
private:
    Randomizer& randomizer;
    ObjectScatterer* scatterer;
    unsigned int treeCount;

    //LEGO pieces
    Importer importer;
    std::map<std::string, std::vector<InstanceInfo>> pieceInstanceInfo;

    Mesh* twoWayMesh;
    Mesh* threeWayMesh;
    Mesh* fourWayMesh;

public:
    Forest(Randomizer& randomizer, ObjectScatterer* scatterer, unsigned int treeCount);
    void populate(unsigned int maxSideSize, std::vector<glm::mat4>& places, std::vector<glm::mat4>& skinning);

    void mutateBranch(ColonBranch* branch, std::vector<glm::mat4>& places, std::vector<glm::mat4>& skinning);
    //void mutateBranchParent(ColonBranch* branch, ColonBranch* child, std::vector<glm::mat4>& skinning);

    //int getCurrentAxis(ColonBranch* branch, const glm::vec3& currentAxis);
    void fillInitialDirections(ColonBranch* branch);

    std::vector<InstanceInfo> getInstanceInfo(std::string key);
    Mesh* getThreeWayMesh();
    Mesh* getTwoWayMesh();
    Mesh* getFourWayMesh();

    ~Forest();
};
