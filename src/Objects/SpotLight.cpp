#include "SpotLight.hpp"

SpotLight::SpotLight(glm::vec3 position, glm::vec3 direction)
{
    this->position = position;
    this->direction = glm::normalize(direction);
}
void SpotLight::setLightColor(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks)
{
    this->ambient = ka;
    this->diffuse = kd;
    this->specular = ks;
}

void SpotLight::setLightIntensity(float constant, float linear, float quadratic)
{
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
}

void SpotLight::setSpotArea(float cutOff, float outerCutOff)
{
    this->cutOff = cutOff;
    this->outerCutOff = outerCutOff;
}

glm::vec3& SpotLight::getPos()
{
    return position;
}
glm::vec3& SpotLight::getDir()
{
    return direction;
}
glm::vec3& SpotLight::getAmbient()
{
    return ambient;
}
glm::vec3& SpotLight::getDiffuse()
{
    return diffuse;
}
glm::vec3& SpotLight::getSpecular()
{
    return specular;
}
float& SpotLight::getLightIntensityC()
{
    return constant;
}
float& SpotLight::getLightIntensityL()
{
    return linear;
}
float& SpotLight::getLightIntensityQ()
{
    return quadratic;
}
float& SpotLight::getLightcutOff()
{
    return cutOff;
}
float& SpotLight::getLightcutOffOuter()
{
    return outerCutOff;
}

SpotLight::~SpotLight()
{}