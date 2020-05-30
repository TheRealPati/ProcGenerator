#include "Forest.hpp"

Forest::Forest(Randomizer& randomizer, ObjectScatterer* scatterer, unsigned int treeCount) : randomizer(randomizer)
{
    this->treeCount = treeCount;
    this->scatterer = scatterer;
}

void Forest::populate(unsigned int maxSideSize, std::vector<glm::mat4>& places, std::vector<glm::mat4>& skinning)
{
    importerTwoWay.loadModel("resources/models/twoway.dae", ModelType::RIGGED);
    importerThreeWay.loadModel("resources/models/threeway.dae", ModelType::RIGGED);

    RegionSeeder seeder = RegionSeeder(randomizer, 200);

    for(unsigned int i = 0; i < treeCount; i++){
        float x = randomizer.rand() * maxSideSize;
        float z = randomizer.rand() * maxSideSize;
        float y = scatterer->getHeight(x,z);

        //Control points for each tree
        std::vector<Point> seedPoints = seeder.seed({x,y+5.0f,z}, 6.0f, 12.0f, FrameRegion::SPHERE);
        
        SpaceColonization colonization = SpaceColonization(1.4f, 3.0f, seedPoints);
        std::vector<ColonBranch*> branches = colonization.colonize({x,y,z}, {0.0f, 1.0f, 0.0f});

        for(ColonBranch* branch : branches)
        {
            mutateBranch(branch, places, skinning);
            if(branch->parent != NULL)
                mutateBranchParent(branch->parent, branch, skinning);

            glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), glm::vec3(branch->pos.x, branch->pos.y, branch->pos.z));
            glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

            places.emplace_back(translateMat * scaleMat);
        }
    }

    threeWayMesh = new RiggedMesh(importerThreeWay.getVertexData(), importerThreeWay.getIndices(), importerThreeWay.getWeights(), threeWay);

    twoWayMesh = new RiggedMesh(importerTwoWay.getVertexData(), importerTwoWay.getIndices(), importerTwoWay.getWeights(), twoWay);
}

void Forest::mutateBranch(ColonBranch* branch, std::vector<glm::mat4>& places, std::vector<glm::mat4>& skinning)
{
    InstanceInfo info;
    info.boneIndices = glm::vec4(0.0f);
    info.modelMatIndex = (float)(places.size());
    glm::vec3 originalAxis = getCurrentAxis(branch);
    glm::mat4 rotMat;

    //Three way
    if(branch->childrenCount == 2)
    {
        rotMat = calcRotMat(originalAxis, glm::normalize(branch->parent->pos - branch->pos));
        info.boneIndices.x = skinning.size();
        
        branch->instanceID = threeWay.size();
        threeWay.emplace_back(info);
    }
    //Two way
    else if(branch->childrenCount == 1 || branch->childrenCount == 0)
    {
        // Root branch
        if(branch->childrenCount == 1 && branch->parent == NULL)
        {
            rotMat = calcRotMat(originalAxis, -branch->direction);
        }
        // Intermediate branch bottom end
        else if(branch->childrenCount == 1)
        {
            rotMat = calcRotMat(originalAxis, glm::normalize(branch->parent->pos - branch->pos));
        }
        // End branch bottom end
        else
        {
            rotMat = calcRotMat(originalAxis, glm::normalize(branch->parent->pos - branch->pos));
        }

        info.boneIndices.x = skinning.size();
        
        branch->instanceID = twoWay.size();
        twoWay.emplace_back(info);
    }
    else
        fprintf(stderr, "TODO megcsinálni %d -ra \n", branch->childrenCount);

    skinning.emplace_back(rotMat);
    branch->leafCount++;
}

void Forest::mutateBranchParent(ColonBranch* branch, ColonBranch* child, std::vector<glm::mat4>& skinning)
{
    glm::vec3 originalAxis = getCurrentAxis(branch);
    glm::mat4 rotMat;

    //Three way
    if(branch->childrenCount == 2)
    {
        rotMat = calcRotMat(originalAxis, glm::normalize(child->pos - branch->pos));
        InstanceInfo& info = threeWay[branch->instanceID];

        if(branch->leafCount == 1)
            info.boneIndices.y = skinning.size();
        if(branch->leafCount == 2)
            info.boneIndices.z = skinning.size();
    }
    //Two way
    else if(branch->childrenCount == 1 || branch->childrenCount == 0)
    {
        rotMat = calcRotMat(originalAxis, glm::normalize(child->pos - branch->pos));
        InstanceInfo& info = twoWay[branch->instanceID];

        if(branch->leafCount == 1)
            info.boneIndices.y = skinning.size();
    }

    skinning.emplace_back(rotMat);
    branch->leafCount++;
}

glm::mat4 Forest::calcRotMat(const glm::vec3& originalAxis, const glm::vec3& modifiedAxis)
{
    glm::vec3 axis = glm::cross(originalAxis, modifiedAxis);
    float angle = acos(glm::dot(originalAxis, modifiedAxis));
    glm::mat4 rotMat = glm::mat4(1.0f);
    if(angle != 0)
        rotMat = glm::rotate(glm::mat4(1.0f), angle, axis);

    return rotMat;
}

glm::vec3 Forest::getCurrentAxis(ColonBranch* branch)
{
    glm::vec3 axis = glm::vec3(0.0f);
    // Two-way
    if(branch->childrenCount == 0 || branch->childrenCount == 1)
    {
        if(branch->leafCount == 0)
            axis = glm::vec3(1.0f, 0.0f, 0.0f);
        else if(branch->leafCount == 1)
            axis = glm::vec3(-1.0f, 0.0f, 0.0f);
    }
    //Threeway
    else if(branch->childrenCount == 2)
    {
        //TODO Kideríteni hogy a fenébe áll
        if(branch->leafCount == 0)
            axis = glm::vec3(0.0f, 0.0f, 1.0f);
        else if(branch->leafCount == 1)
            axis = glm::normalize(glm::vec3(0.0f, 1.0f, -1.0f));
        else if(branch->leafCount == 2)
            axis = glm::normalize(glm::vec3(0.0f, -1.0f, -1.0f));
    }

    return axis;
}

std::vector<InstanceInfo> Forest::getThreeWayInstances()
{
    return threeWay;
}

std::vector<InstanceInfo> Forest::getTwoWayInstances()
{
    return twoWay;
}

Mesh* Forest::getThreeWayMesh()
{
    return threeWayMesh;
}

Mesh* Forest::getTwoWayMesh()
{
    return twoWayMesh;
}