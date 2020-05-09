#include "Material.hpp"

Material::Material(Shader* shader)
{
    this->shader = shader;
}

void Material::loadTexture(std::string filePath)
{
    this->filePath = filePath;
    glGenTextures(1, &diffuse);
    glBindTexture(GL_TEXTURE_2D, diffuse); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); 
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        fprintf(stderr, "[ERROR] Texture loading failed : %s!\n", filePath.c_str() );
    }
    stbi_image_free(data);
}

void Material::setMaterialProps(glm::vec3 specular, float shininess)
{
    this->specular = specular;
    this->shininess = shininess;
}

std::string Material::getName()
{
    return filePath;
}

void Material::draw()
{
    shader->use();
    glBindTexture(GL_TEXTURE_2D, diffuse);
    glActiveTexture(GL_TEXTURE0);
    shader->setInt("material.diffuse", 0);
    shader->setVec3("material.specular", specular);
    shader->setFloat("material.shininess", shininess);
}

void Material::setModelMatrix(glm::mat4& model, glm::mat4& normal)
{
    shader->setMatrix("model", model);
    shader->setMatrix("normalMatrix", normal);
}

void Material::setCameraPos(glm::vec3& pos)
{
    shader->setVec3("camera.positon", pos);
}
