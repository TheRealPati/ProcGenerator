#include "MaterialSingle.hpp"

MaterialSingle::MaterialSingle(Shader* shader) : Material(shader){}

MaterialSingle::~MaterialSingle()
{
    delete generatedData;
}

void MaterialSingle::loadTexture(std::string filePath, int colorMode)
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

void MaterialSingle::initBuffer()
{
    glGenTextures(1, &diffuse);
    glBindTexture(GL_TEXTURE_2D, diffuse); 

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void MaterialSingle::generateTexture(int width, int height)
{
    this->initBuffer();

    NoiseGen noiseGen = NoiseGen();
    generatedData = new std::vector<float>();
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            float intensity = noiseGen.woodValue(glm::vec2((double)i, (double)j));
            generatedData->emplace_back(intensity);
            generatedData->emplace_back(intensity);
            generatedData->emplace_back(intensity);
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, &(*generatedData)[0]);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void MaterialSingle::setMaterialProps(glm::vec3 specular, float shininess)
{
    this->specular = specular;
    this->shininess = shininess;
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

