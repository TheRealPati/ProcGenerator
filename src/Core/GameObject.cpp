#include "GameObject.hpp"

GameObject::GameObject()
{

}

void GameObject::draw(Camera* camera)
{
    normalMatrix = glm::inverse(glm::transpose(modelMatrix * camera->getView()));
    model->draw(modelMatrix, normalMatrix, camera->getPos());
}

void GameObject::setModel(Model* model)
{
    this->model = model;
}

void GameObject::setMatrix(glm::mat4 matrix)
{
    modelMatrix = matrix;
}

GameObject::~GameObject()
{

}