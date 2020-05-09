#include "PointLight.hpp"

PointLight::PointLight(glm::vec3 position) : position(position){}

void PointLight::setLightColor(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks)
{
    this->ambient = ka;
    this->diffuse = kd;
    this->specular = ks;
}

void PointLight::setLightIntensity(float constant, float linear, float quadratic)
{
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
}

glm::vec3& PointLight::getPos()
{
    return position;
}
glm::vec3& PointLight::getAmbient()
{
    return ambient;
}
glm::vec3& PointLight::getDiffuse()
{
    return diffuse;
}
glm::vec3& PointLight::getSpecular()
{
    return specular;
}
float& PointLight::getLightIntensityC()
{
    return constant;
}
float& PointLight::getLightIntensityL()
{
    return linear;
}
float& PointLight::getLightIntensityQ()
{
    return quadratic;
}

PointLight::~PointLight()
{}