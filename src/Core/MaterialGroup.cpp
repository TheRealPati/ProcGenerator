#include "MaterialGroup.hpp"

MaterialGroup::MaterialGroup(Shader* shader) : Material(shader){}

MaterialGroup::~MaterialGroup()
{
    delete generatedData;
}

void MaterialGroup::loadTexture(std::string filePath, int colorMode)
{
    this->filePath = filePath;
    this->initBuffer();

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); 
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        fprintf(stderr, "[ERROR] Texture loading failed : %s!\n", filePath.c_str() );
    }
    stbi_image_free(data);
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

void MaterialGroup::generateTexture(int width, int height)
{
    this->initBuffer();

    NoiseGen noiseGen = NoiseGen();
    generatedData = new std::vector<float>();
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            float intensity = noiseGen.woodValue(glm::vec2((double)i, (double)j));
            //TODO Tweak
            if(intensity > 0.4f) intensity *= 0.3f;
            generatedData->emplace_back(intensity);
            generatedData->emplace_back(intensity);
            generatedData->emplace_back(intensity);
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, &(*generatedData)[0]);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void MaterialGroup::setMaterialProps(glm::vec3 specular, float shininess)
{
    this->specular = specular;
    this->shininess = shininess;
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

