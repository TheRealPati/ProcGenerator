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
        branch->directions = {{0.0f, 0.0f, 1.0f},{0.0f, 1.0f, 0.0f},{0.0f, 0.0f, -1.0f},{0.0f, -1.0f, 0.0f}};
    }
}

void Forest::populate(unsigned int maxSideSize, std::vector<glm::mat4>& places, std::vector<glm::mat4>& skinning, std::vector<glm::mat4>& leafBillboard)
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
        std::vector<PN> seedPoints = seeder.seed(point, 4.0f, 8.0f, FrameRegion::SPHERE);
        
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

            if(branch->childrenCount == 0){
                for(int i = 0; i < 5; i++){
                    glm::mat4 randRot = scatterer->calcRotMat(upVector, glm::vec3(randomizer.randCenterEqually(), randomizer.randCenterEqually(), randomizer.randCenterEqually()));
                    leafBillboard.emplace_back(translateMat * randRot);
                }
            }

            mutateBranch(branch, places, skinning);

            places.emplace_back(translateMat * scaleMat /** rotMat*/);
        }
    }

}

std::vector<glm::vec3> calcNearestAngles(ColonBranch* branch)
{

    //Last pieces
    if(branch->childrenCount == 0){
        //fprintf(stderr, "LAST PIECE\n");
        branch->childDirections.push_back(branch->direction);
    }

    //Parent side
    if(branch->parent != NULL){
        //fprintf(stderr, "PARENT\n");
        branch->childDirections.push_back(glm::normalize(branch->parent->pos - branch->pos));
    }
    else{
        //fprintf(stderr, "NINCS PARENT\n");
        branch->childDirections.push_back(-branch->direction);
    }

    std::vector<std::tuple<float, int, int>> angleMatrix = {};
    for(unsigned int i = 0; i < branch->directions.size(); i++)
    {
        glm::vec3& dir = branch->directions[i];
        //fprintf(stderr, "%d -DIK PIECE\n", i);

        for(unsigned int j = 0; j < branch->childDirections.size(); j++)
        {
            glm::vec3& childDir = branch->childDirections[j];
            angleMatrix.push_back(std::tuple<float, int, int>(acos(glm::dot(childDir, dir)), i, j));
            //fprintf(stderr, "%f\n", acos(glm::dot(childDir, dir)));
        }
    }
    //fprintf(stderr, "\n\n\n");

    //Sorrendezes
    const int numOfDirections = branch->directions.size();
    sort(angleMatrix.begin(), angleMatrix.end(),
    [](std::tuple<float, int, int> const &t1, std::tuple<float, int, int> const &t2) {
        return std::get<0>(t1) > std::get<0>(t2);
    }); 

    for (int i = 0; i < angleMatrix.size(); i++){
        //fprintf(stderr, "[ %f, %d, %d ]\n", std::get<0>(angleMatrix[i]), std::get<1>(angleMatrix[i]), std::get<2>(angleMatrix[i]));
    } 

    std::vector<int> found(numOfDirections);
    //std::vector<int> branchChooseOrder(numOfDirections);
    std::vector<int> childDirChooseOrder(numOfDirections);
    int i=0;

    std::vector<int> splittedBranches[numOfDirections];

    for(const auto& angleTuple : angleMatrix){
        //Correct order
        found[std::get<2>(angleTuple)] += 1;
        if(found[std::get<2>(angleTuple)] == numOfDirections){
            //fprintf(stderr, "%d\n", std::get<1>(angleTuple));
            childDirChooseOrder[i] = std::get<2>(angleTuple);
            i++;
        }

        //Splitting them - max in first place
        splittedBranches[std::get<2>(angleTuple)].emplace_back(std::get<1>(angleTuple));
    }

    for (int i = 0; i < numOfDirections; i++){
        //fprintf(stderr, "{");
        for (int j = 0; j < numOfDirections; j++){
            //fprintf(stderr, " %d ", splittedBranches[i][j]);
        } 
        //fprintf(stderr, "}\n");
    }

    std::vector<glm::vec3> result(numOfDirections);
    std::vector<int> outOfOrder = {};
    //Bejárás hátulról
    //Itt valami nem stimmel
    //Ha egy childdirt mindannyian megtaláltak akkor a legjobb direction megkapja - Ez a V1
    for(int i = 0; i < numOfDirections; i++){
        int choosenIndex = childDirChooseOrder[i];
        //fprintf(stderr, "INDEX: %d\n", choosenIndex);
        bool found = false;
        for(int j = splittedBranches[choosenIndex].size() - 1; j >= 0 ; j--){
            auto it = std::find(outOfOrder.begin(), outOfOrder.end(), splittedBranches[choosenIndex][j]); 
            if (!found && it == outOfOrder.end()) {
                found = true;
                int originalBranchIndex = splittedBranches[choosenIndex][j];
                outOfOrder.push_back(originalBranchIndex);
                //fprintf(stderr, "BESZÚROM: (%d - %d)\n", originalBranchIndex, choosenIndex);
                result[originalBranchIndex] = branch->childDirections[choosenIndex];
            }
        }
    }

    return result;
}

glm::mat4 Forest::scaleEndPiece(glm::mat4& mat)
{
    return glm::scale(mat, glm::vec3(0.35f));
}

void Forest::mutateBranch(ColonBranch* branch, std::vector<glm::mat4>& places, std::vector<glm::mat4>& skinning)
{
    if(branch->childrenCount < 5){
        InstanceInfo info;
        info.boneIndices = glm::vec4(0.0f);
        info.modelMatIndex = (float)(places.size());

        //Kapok map[int => modified direction] mapet
        std::vector<glm::vec3> nearestAngles = calcNearestAngles(branch);

        for(unsigned int i = 0; i < nearestAngles.size(); i++)
        {
            glm::mat4 rotMat;
            rotMat = scatterer->calcRotMat(branch->directions[i], nearestAngles[i]);
            if(branch->childrenCount == 0 && nearestAngles[i].y > 0){
                rotMat = scaleEndPiece(rotMat);
            }
        
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

Forest::~Forest()
{
    
}