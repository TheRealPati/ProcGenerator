#include "Forest.hpp"

Forest::Forest(Randomizer& randomizer, ObjectScatterer* scatterer, unsigned int treeCount) : randomizer(randomizer)
{
    this->treeCount = treeCount;
    this->scatterer = scatterer;
}

void Forest::populate(unsigned int maxSideSize, std::vector<glm::mat4>& places, std::vector<glm::mat4>& skinning)
{
    RegionSeeder seeder = RegionSeeder(randomizer, 200);

    for(unsigned int i = 0; i < treeCount; i++){
        float x = randomizer.rand() * maxSideSize;
        float z = randomizer.rand() * maxSideSize;
        float y = scatterer->getHeight(x,z);

        //Control points for each tree
        PN point;
        point.position = {x,y+5.0f,z};
        std::vector<PN> seedPoints = seeder.seed(point, 6.0f, 12.0f, FrameRegion::SPHERE);
        
        SpaceColonization colonization = SpaceColonization(1.4f, 3.0f, seedPoints);
        std::vector<ColonBranch*> branches = colonization.colonize({x,y,z}, scatterer->getNormal(x,z));

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

    importer.loadModel("resources/models/2way_rig.dae", ModelType::RIGGED);
    twoWayMesh = new RiggedMesh(importer.getVertexData(), importer.getIndices(), importer.getWeights(), pieceInstanceInfo["twoWay"]);

    importer.loadModel("resources/models/3way_rig.dae", ModelType::RIGGED);
    threeWayMesh = new RiggedMesh(importer.getVertexData(), importer.getIndices(), importer.getWeights(), pieceInstanceInfo["threeWay"]);

    importer.loadModel("resources/models/4way_rig.dae", ModelType::RIGGED);
    fourWayMesh = new RiggedMesh(importer.getVertexData(), importer.getIndices(), importer.getWeights(), pieceInstanceInfo["fourWay"]);

}

void Forest::mutateBranch(ColonBranch* branch, std::vector<glm::mat4>& places, std::vector<glm::mat4>& skinning)
{
    InstanceInfo info;
    info.boneIndices = glm::vec4(0.0f);
    info.modelMatIndex = (float)(places.size());
    glm::vec3 originalAxis = getCurrentAxis(branch);
    glm::mat4 rotMat;

    //Four way
    if(branch->childrenCount == 3)
    {
        rotMat = scatterer->calcRotMat(originalAxis, glm::normalize(branch->parent->pos - branch->pos));
        info.boneIndices.x = skinning.size();
        
        branch->instanceID = pieceInstanceInfo["fourWay"].size();
        pieceInstanceInfo["fourWay"].emplace_back(info);
    }
    //Three way
    else if(branch->childrenCount == 2)
    {
        rotMat = scatterer->calcRotMat(originalAxis, glm::normalize(branch->parent->pos - branch->pos));
        info.boneIndices.x = skinning.size();
        
        branch->instanceID = pieceInstanceInfo["threeWay"].size();
        pieceInstanceInfo["threeWay"].emplace_back(info);
    }
    //Two way
    else if(branch->childrenCount == 1 || branch->childrenCount == 0)
    {
        // Root branch
        if(branch->childrenCount == 1 && branch->parent == NULL)
        {
            rotMat = scatterer->calcRotMat(originalAxis, -branch->direction);
        }
        // Intermediate branch bottom end
        else if(branch->childrenCount == 1)
        {
            rotMat = scatterer->calcRotMat(originalAxis, glm::normalize(branch->parent->pos - branch->pos));
        }
        // End branch bottom end
        else
        {
            rotMat = scatterer->calcRotMat(originalAxis, glm::normalize(branch->parent->pos - branch->pos));
        }

        info.boneIndices.x = skinning.size();
        
        branch->instanceID = pieceInstanceInfo["twoWay"].size();
        pieceInstanceInfo["twoWay"].emplace_back(info);
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

    //Four way
    if(branch->childrenCount == 3)
    {
        InstanceInfo& info = pieceInstanceInfo["fourWay"][branch->instanceID];

        if(branch->leafCount == 1)
            info.boneIndices.y = skinning.size();
        if(branch->leafCount == 2)
            info.boneIndices.z = skinning.size();
        if(branch->leafCount == 3)
            info.boneIndices.w = skinning.size();
    }
    //Three way
    if(branch->childrenCount == 2)
    {
        InstanceInfo& info = pieceInstanceInfo["threeWay"][branch->instanceID];

        if(branch->leafCount == 1)
            info.boneIndices.y = skinning.size();
        if(branch->leafCount == 2)
            info.boneIndices.z = skinning.size();
    }
    //Two way
    else if(branch->childrenCount == 1 || branch->childrenCount == 0)
    {
        InstanceInfo& info = pieceInstanceInfo["twoWay"][branch->instanceID];

        if(branch->leafCount == 1)
            info.boneIndices.y = skinning.size();
    }

    rotMat = scatterer->calcRotMat(originalAxis, glm::normalize(child->pos - branch->pos));
    skinning.emplace_back(rotMat);
    branch->leafCount++;
}



glm::vec3 Forest::getCurrentAxis(ColonBranch* branch)
{
    glm::vec3 axis = glm::vec3(0.0f);
    // Two-way
    if(branch->childrenCount == 0 || branch->childrenCount == 1)
    {
        if(branch->leafCount == 0)
            axis = glm::vec3(0.0f, 0.0f, 1.0f);
        else if(branch->leafCount == 1)
            axis = glm::vec3(0.0f, 0.0f, -1.0f);
    }
    //Threeway
    else if(branch->childrenCount == 2)
    {
        if(branch->leafCount == 0)
            axis = glm::vec3(0.0f, 0.0f, 1.0f);
        else if(branch->leafCount == 1)
            axis = glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f));
        else if(branch->leafCount == 2)
            axis = glm::normalize(glm::vec3(-1.0f, 0.0f, -1.0f));
    }
    //Fourway
    else if(branch->childrenCount == 3)
    {
        if(branch->leafCount == 0)
            axis = glm::vec3(0.0f, 0.0f, 1.0f);
        else if(branch->leafCount == 1)
            axis = glm::vec3(1.0f, 0.0f, 0.0f);
        else if(branch->leafCount == 2)
            axis = glm::vec3(0.0f, 0.0f, -1.0f);
        else if(branch->leafCount == 3)
            axis = glm::vec3(-1.0f, 0.0f, 0.0f);
    }

    return axis;
}

std::vector<InstanceInfo> Forest::getInstanceInfo(std::string key)
{
    return pieceInstanceInfo.at(key);
}

Mesh* Forest::getThreeWayMesh()
{
    return threeWayMesh;
}

Mesh* Forest::getTwoWayMesh()
{
    return twoWayMesh;
}

Mesh* Forest::getFourWayMesh()
{
    return fourWayMesh;
}