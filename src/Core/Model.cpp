#include "Model.hpp"

Model::Model()
{

}

void Model::addMat(Material* mat, GLuint index)
{
    meshMatMap.insert(std::pair<GLuint, GLuint>(index, materials.size()));
    materials.push_back(mat);
}

void Model::addMesh(Mesh* mesh)
{
    meshes.push_back(mesh);
}

void Model::draw(glm::mat4& model, glm::mat4& normal, glm::vec3& cameraPos, int count)
{
    for(unsigned int i = 0; i < meshes.size(); i++)
    {
        unsigned int index = meshMatMap.at(i);
        materials[index]->draw();
        materials[index]->setCameraPos(cameraPos);
        materials[index]->setModelMatrix(model, normal);
        meshes[i]->draw(count);
    }
}

Model::~Model()
{

}