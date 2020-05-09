#include "DirectionalLight.hpp"

DirectionalLight::DirectionalLight(glm::vec3 direction){
    this->direction = glm::normalize(direction);
}

void DirectionalLight::setLightColor(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks)
{
    this->ambient = ka;
    this->diffuse = kd;
    this->specular = ks;
}

glm::vec3& DirectionalLight::getDir()
{
    return direction;
}
glm::vec3& DirectionalLight::getAmbient()
{
    return ambient;
}
glm::vec3& DirectionalLight::getDiffuse()
{
    return diffuse;
}
glm::vec3& DirectionalLight::getSpecular()
{
    return specular;
}

DirectionalLight::~DirectionalLight()
{}