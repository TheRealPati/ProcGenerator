#include "MaterialSingle.hpp"

MaterialSingle::MaterialSingle(Shader* shader) : Material(shader){}

MaterialSingle::~MaterialSingle()
{
    delete generatedData;
}

void MaterialSingle::initBuffer()
{
    glGenTextures(1, &diffuse);
    glBindTexture(GL_TEXTURE_2D, diffuse); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void MaterialSingle::draw()
{
    shader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuse);
    shader->setInt("material.diffuse", 0);
    shader->setVec3("material.specular", specular);
    shader->setFloat("material.shininess", shininess);
}

