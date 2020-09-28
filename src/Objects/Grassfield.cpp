#include "Grassfield.hpp"

Grassfield::Grassfield(Randomizer& randomizer, ObjectScatterer* scatterer) : randomizer(randomizer)
{
    this->scatterer = scatterer;
}

void Grassfield::populate(unsigned int maxSideSize, std::vector<glm::mat4>& billboardModelMatrixes, unsigned int grassCount)
{
    glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    for(unsigned int i = 0; i < grassCount; i++){
        float x = randomizer.rand() * maxSideSize;
        float z = randomizer.rand() * maxSideSize;
        float y = scatterer->getHeight(x,z);

        glm::mat4 transMat = glm::mat4(1.0f);
        glm::vec3 axis = scatterer->getNormal(x,z);
        transMat = glm::translate(transMat, glm::vec3(x,y,z) - axis * 0.1f);
        transMat *= scatterer->calcRotMat(upVector, axis);
        float angle = randomizer.rand() * PI;
        if(angle != 0)
            transMat = glm::rotate(transMat, angle, glm::vec3(0.0f, 1.0f, 0.0f));

        billboardModelMatrixes.emplace_back(transMat);
    }

}