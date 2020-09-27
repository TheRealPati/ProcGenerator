#include "MaterialGroup.hpp"

MaterialGroup::MaterialGroup(Shader* shader) : Material(shader){}

MaterialGroup::~MaterialGroup()
{
    delete generatedData;
}

void MaterialGroup::initBuffer()
{
    glGenTextures(1, &currentID);
    glBindTexture(GL_TEXTURE_2D, currentID); 
    texIDs.push_back(currentID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void MaterialGroup::draw()
{
    shader->use();
    for(unsigned int i = 0; i < texIDs.size(); i++)
    {
        shader->setInt("texture" + std::to_string(i) + ".texMap", i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texIDs[i]);
    }
    shader->setVec3("material.specular", specular);
    shader->setFloat("material.shininess", shininess);
}

