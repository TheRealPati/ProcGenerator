#include "SpaceColonization.hpp"

SpaceColonization::SpaceColonization(float minDistance, float maxDistance, std::vector<PN>& points) : points(points)
{
    this->minDistance = minDistance;
    this->maxDistance = maxDistance;
}

std::vector<ColonBranch*> SpaceColonization::colonize(glm::vec3 rootPos, glm::vec3 rootNormal)
{
    std::vector<ColonBranch*> generatedBranches = {};

    ColonBranch* root = new ColonBranch();
    root->pos = rootPos;
    root->direction = rootNormal;
    generatedBranches.emplace_back(root);

    bodyGrow(root, generatedBranches);
    for(int i = 0; i < 80; i++){
        leafGrow(generatedBranches);
    }
    
    return generatedBranches;
}

void SpaceColonization::bodyGrow(ColonBranch* root, std::vector<ColonBranch*>& generatedBranches)
{
    ColonBranch* current = root;

    //Root to the first point
    bool firstLeafFound = false;
    while(!firstLeafFound)
    {
        for(PN point : points)
        {
            glm::vec3 pointPos(point.position.x, point.position.y, point.position.z);
            float distance = glm::length(pointPos - current->pos);
            if(distance < maxDistance)
                firstLeafFound = true;
        }

        if(!firstLeafFound)
        {
            glm::vec3 newDir = glm::normalize(current->direction + glm::vec3(0.0f, 1.0f, 0.0f));
            glm::vec3 newPos = current->pos + newDir * 0.385f;
            ColonBranch* branch = new ColonBranch({newPos, newDir, newDir, current});
            current->childrenCount++;
            generatedBranches.emplace_back(branch);
            current = branch;
        }
    }
}

void SpaceColonization::leafGrow(std::vector<ColonBranch*>& generatedBranches)
{
    std::vector<int> killable;
    //First leaf found, scattering phase
    for(unsigned int i = 0; i < points.size(); i++)
    {
        ColonBranch* closest = nullptr;
        glm::vec3 pointPos(points[i].position.x, points[i].position.y, points[i].position.z);
        float closestDistance = maxDistance;
        for(ColonBranch* branch : generatedBranches)
        {
            float distance = glm::length(pointPos - branch->pos);
            if(distance < minDistance)
            {
                killable.emplace_back(i);
                closest = nullptr;
                break;
            }
            else if(distance <= closestDistance)
            {
                closestDistance = distance;
                closest = branch;
            }
        }

        if(closest != nullptr)
        {
            glm::vec3 leafDir = glm::normalize(pointPos - closest->pos);
            glm::vec3 newDir = glm::normalize(leafDir + closest->direction);
            closest->direction = newDir;
            closest->leafCount++;
        }
    }

    for(int i = killable.size()-1; i >= 0; i--)
    {
        points.erase(points.begin() + killable[i]);
    }

    std::vector<ColonBranch*> addable = {};
    for(ColonBranch* branch : generatedBranches)
    {
        if (branch->leafCount > 0) {
            glm::vec3 newDir = glm::normalize(branch->direction + glm::vec3(0.0f, 1.0f, 0.0f));
            glm::vec3 newPos = branch->pos + branch->direction * 0.385f;
            ColonBranch* newBranch = new ColonBranch({newPos, newDir, newDir, branch});
            branch->childrenCount++;
            addable.emplace_back(newBranch);
            branch->direction = branch->original_direction;
            branch->leafCount = 0;
        }
    }
    generatedBranches.insert(generatedBranches.end(), addable.begin(), addable.end());
    addable.clear();
}

SpaceColonization::~SpaceColonization()
{
}