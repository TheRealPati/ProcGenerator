#include "Material.hpp"

Material::Material(Shader* shader)
{
    this->shader = shader;
}

Material::~Material() {}

void Material::loadTexture(std::string filePath, int colorMode)
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

void Material::generateTexture(int width, int height)
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

void Material::setMaterialProps(glm::vec3 specular, float shininess)
{
    this->specular = specular;
    this->shininess = shininess;
}

void Material::setCameraPos(glm::vec3& pos)
{
    shader->setVec3("camera.positon", pos);
}

void Material::setModelMatrix(glm::mat4& model, glm::mat4& normal)
{
    shader->setMatrix("model", model);
    shader->setMatrix("normalMatrix", normal);
}

