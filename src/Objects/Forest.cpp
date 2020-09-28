#include "Forest.hpp"

Forest::Forest(Randomizer& randomizer, ObjectScatterer* scatterer, unsigned int treeCount) : randomizer(randomizer)
{
    this->treeCount = treeCount;
    this->scatterer = scatterer;
}

void Forest::fillInitialDirections(ColonBranch* branch)
{
    if(branch->childrenCount == 0 || branch->childrenCount == 1)
    {
        branch->directions = {{0.0f, 0.0f, 1.0f},{0.0f, 0.0f, -1.0f}};
    }
    else if(branch->childrenCount == 2)
    {
        branch->directions = {{0.0f, 0.0f, 1.0f},glm::normalize(glm::vec3(0.0f, 1.0f, -1.0f)),glm::normalize(glm::vec3(0.0f, -1.0f, -1.0f))};
    }
    else if(branch->childrenCount == 3)
    {
        branch->directions = {{0.0f, 0.0f, 1.0f},{1.0f, 0.0f, 0.0f},{0.0f, 0.0f, -1.0f},{-1.0f, 0.0f, 0.0f}};
    }
}

void Forest::populate(unsigned int maxSideSize, std::vector<glm::mat4>& places, std::vector<glm::mat4>& skinning)
{
    RegionSeeder seeder = RegionSeeder(randomizer, 200);
    glm::vec3 upVector = glm::vec3(0.0f, 0.0f, -1.0f);

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
            this->fillInitialDirections(branch);
            if(branch->parent != NULL)
                branch->parent->childDirections.emplace_back(glm::normalize(branch->pos - branch->parent->pos));
        }

        for(ColonBranch* branch : branches)
        {

            glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), branch->pos);
            glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

            glm::vec3 axis = glm::cross(upVector, branch->direction);
            float angle = acos(glm::dot(upVector, branch->direction));
            glm::mat4 rotMat = glm::mat4(1.0f);
            if(angle != 0)
            {
                rotMat = glm::rotate(glm::mat4(1.0f), angle, axis);
            }
            // for(auto& piece : branch->directions)
            // {
            //     piece = glm::vec3(rotMat * glm::vec4(piece, 1.0));
            // }

            mutateBranch(branch, places, skinning);

            places.emplace_back(translateMat * scaleMat /** rotMat*/);
        }
    }

    importer.loadModel("resources/models/2_generated.dae", ModelType::RIGGED);
    twoWayMesh = new RiggedMesh(importer.getVertexData(), importer.getIndices(), importer.getWeights(), pieceInstanceInfo["twoWay"]);

    importer.loadModel("resources/models/3_generated.dae", ModelType::RIGGED);
    threeWayMesh = new RiggedMesh(importer.getVertexData(), importer.getIndices(), importer.getWeights(), pieceInstanceInfo["threeWay"]);

    importer.loadModel("resources/models/4way_rig.dae", ModelType::RIGGED);
    fourWayMesh = new RiggedMesh(importer.getVertexData(), importer.getIndices(), importer.getWeights(), pieceInstanceInfo["fourWay"]);

}

std::vector<glm::vec3> calcNearestAngles(ColonBranch* branch)
{
    std::vector<std::map<float, glm::vec3>> angleMatrix;
    int i = 0;
    for(auto& direction : branch->directions)
    {
        angleMatrix.push_back({});
        for(auto& childDir : branch->childDirections)
        {
            angleMatrix[i].insert ( std::pair<float, glm::vec3>(acos(glm::dot(childDir, direction)), childDir));
            fprintf(stderr, "%f\n", acos(glm::dot(childDir, direction)));
        }
        if(branch->parent != NULL)
            angleMatrix[i].insert ( std::pair<float, glm::vec3>(acos(glm::dot(glm::normalize(branch->parent->pos - branch->pos), direction)), glm::normalize(branch->parent->pos - branch->pos)));
        i++;
    }
    fprintf(stderr, "\n\n\n");

    //Minimumkeresés
    std::vector<glm::vec3> result = {};
    std::vector<glm::vec3> outOfOrder = {};
    for(unsigned int i = 0; i < angleMatrix.size(); i++)
    {
        float minValue = 500.0f;
        glm::vec3 correctDir = glm::vec3(1.0f);
        for (const auto &pair : angleMatrix[i]) 
        {
            fprintf(stderr, "%f,%f,%f - %f\n", pair.second.x, pair.second.y, pair.second.z, pair.first);
            if(pair.first < minValue && std::find_if(outOfOrder.begin(), outOfOrder.end(), [&](const auto& val){ 
                const double epsilion = 0.001f;

                fprintf(stderr, "Am - %d\n", fabs(val.x -pair.second.x) < epsilion   
                && fabs(val.y -pair.second.y) < epsilion   
                && fabs(val.z -pair.second.z) < epsilion);

                return fabs(val.x -pair.second.x) < epsilion   
                && fabs(val.y -pair.second.y) < epsilion   
                && fabs(val.z -pair.second.z) < epsilion; 
            }) == outOfOrder.end())
            {
                fprintf(stderr, "Ez:e(%d) %f,%f,%f - %f\n",i, pair.second.x, pair.second.y, pair.second.z, pair.first);
                minValue = pair.first;
                correctDir = pair.second;
            }
        }
        result.push_back(correctDir);
        outOfOrder.push_back(correctDir);
    }
    fprintf(stderr, "\n\n\n");

    return result;
}

void Forest::mutateBranch(ColonBranch* branch, std::vector<glm::mat4>& places, std::vector<glm::mat4>& skinning)
{
    if(branch->childrenCount < 5){
        InstanceInfo info;
        info.boneIndices = glm::vec4(0.0f);
        info.modelMatIndex = (float)(places.size());

        //Kapok map[int => modified direction] mapet
        std::vector<glm::vec3> nearestAngles = calcNearestAngles(branch);

        //Foreach directions{
        for(unsigned int i = 0; i < nearestAngles.size(); i++)
        {
            glm::mat4 rotMat;
            rotMat = scatterer->calcRotMat(branch->directions[i], nearestAngles[i]);
        
            if(i == 0)
                info.boneIndices.x = skinning.size();
            else if(i == 1)
                info.boneIndices.y = skinning.size();
            else if(i == 2)
                info.boneIndices.z = skinning.size();
            else if(i == 3)
                info.boneIndices.w = skinning.size();

            skinning.emplace_back(rotMat);
            branch->leafCount++;
        }

        if(branch->childrenCount == 0 || branch->childrenCount == 1)
        {
            branch->instanceID = pieceInstanceInfo["twoWay"].size();
            pieceInstanceInfo["twoWay"].emplace_back(info);
        }
        else if(branch->childrenCount == 2)
        {
            branch->instanceID = pieceInstanceInfo["threeWay"].size();
            pieceInstanceInfo["threeWay"].emplace_back(info);
        }
        else if(branch->childrenCount == 3)
        {
            branch->instanceID = pieceInstanceInfo["fourWay"].size();
            pieceInstanceInfo["fourWay"].emplace_back(info);
        }
    }
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