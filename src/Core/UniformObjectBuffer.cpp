#include "UniformObjectBuffer.hpp"

UniformBufferObject::UniformBufferObject(GLuint bindingPoint)
{
    this->bindingPoint = bindingPoint;
    glGenBuffers(1, &UBO);
}

void UniformBufferObject::preserveVPMatrix()
{
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, UBO, 0, 2 * sizeof(glm::mat4));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::preservePointLight(int countOfLamps)
{
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, countOfLamps * 4 * sizeof(glm::vec4) + countOfLamps * 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, UBO, 0, countOfLamps * 4 * sizeof(glm::vec4) + countOfLamps * 3 * sizeof(float));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::preserveDirectionalLight()
{
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, UBO, 0, 4 * sizeof(glm::vec4));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::preserveSpotLight()
{
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, 5 * sizeof(glm::vec4) + 5 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, UBO, 0, 5 * sizeof(glm::vec4) + 5 * sizeof(float));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::setVPmatrix(glm::mat4 view, glm::mat4 projection)
{
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    //0 offset
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
    //64 offset
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::setDirectionLights(DirectionalLight* light)
{
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    //0 offset
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(light->getDir()));
    //16 offset
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(light->getAmbient()));
    //32 offset
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(light->getDiffuse()));
    //48 offset
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(light->getSpecular()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBufferObject::setPointLights(PointLight* light, int index)
{
    int maxSize = index * (4 * sizeof(glm::vec4) + 3 * sizeof(float));
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    //0 offset
    glBufferSubData(GL_UNIFORM_BUFFER, maxSize + 0, sizeof(glm::vec4), glm::value_ptr(light->getPos()));
    //16 offset
    glBufferSubData(GL_UNIFORM_BUFFER, maxSize + sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(light->getAmbient()));
    //32 offset
    glBufferSubData(GL_UNIFORM_BUFFER, maxSize + 2 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(light->getDiffuse()));
    //48 offset
    glBufferSubData(GL_UNIFORM_BUFFER, maxSize + 3 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(light->getSpecular()));
    //64 offset
    glBufferSubData(GL_UNIFORM_BUFFER, maxSize + 4 * sizeof(glm::vec4), sizeof(float), &light->getLightIntensityC());
    //68 offset
    glBufferSubData(GL_UNIFORM_BUFFER, maxSize + 4 * sizeof(glm::vec4) + sizeof(float), sizeof(float), &light->getLightIntensityL());
    //72 offset
    glBufferSubData(GL_UNIFORM_BUFFER, maxSize + 4 * sizeof(glm::vec4) + 2 * sizeof(float), sizeof(float), &light->getLightIntensityQ());
}

void UniformBufferObject::setSpotLights(SpotLight* light)
{
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    //0 offset
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(light->getPos()));
    //16 offset
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(light->getDir()));
    //32 offset
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(light->getAmbient()));
    //48 offset
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(light->getDiffuse()));
    //64 offset
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(light->getSpecular()));
    //80 offset
    glBufferSubData(GL_UNIFORM_BUFFER, 5 * sizeof(glm::vec4), sizeof(float), &light->getLightIntensityC());
    //84 offset
    glBufferSubData(GL_UNIFORM_BUFFER, 5 * sizeof(glm::vec4) + sizeof(float), sizeof(float), &light->getLightIntensityL());
    //88 offset
    glBufferSubData(GL_UNIFORM_BUFFER, 5 * sizeof(glm::vec4) + 2 * sizeof(float), sizeof(float), &light->getLightIntensityQ());
    //92 offset
    glBufferSubData(GL_UNIFORM_BUFFER, 5 * sizeof(glm::vec4) + 3 * sizeof(float), sizeof(float), &light->getLightcutOff());
    //96 offset
    glBufferSubData(GL_UNIFORM_BUFFER, 5 * sizeof(glm::vec4) + 4 * sizeof(float), sizeof(float), &light->getLightcutOffOuter());
    
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBufferObject::~UniformBufferObject()
{
    glDeleteBuffers(1, &UBO);
}