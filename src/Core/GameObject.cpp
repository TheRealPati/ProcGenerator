#include "GameObject.hpp"

GameObject::GameObject()
{

}

void GameObject::draw(Camera* camera)
{
    normalMatrix = glm::transpose(glm::inverse(modelMatrix));
    model->draw(modelMatrix, normalMatrix, camera->getPos(), instanceCount);
}

void GameObject::setModel(Model* model)
{
    this->model = model;
}

void GameObject::setMatrix(glm::mat4 matrix)
{
    modelMatrix = matrix;
}

void GameObject::setInstanceCount(int count)
{
    instanceCount = count;
}

GameObject::~GameObject()
{
    if(model)
        delete model;
}