#include "Importer.hpp"

Importer::Importer()
{

}

void Importer::loadModel(std::string path, ModelType type)
{

    vertexData.clear();
    indices.clear();
	boneMapping.clear();
    vertexBoneData.clear();
    offsetMatrixes.clear();

    this->type = type;
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(FileSystem::getPath(path), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        fprintf(stderr, "[ERROR] Assimp: %s!\n", importer.GetErrorString() );
        return;
    }

    processNode(scene->mRootNode, scene);
}

void Importer::processNode(aiNode *node, const aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh);
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

glm::mat4 Importer::AiToGLMMat4(aiMatrix4x4& in_mat)
{
    glm::mat4 tmp;
    tmp[0][0] = in_mat.a1;
    tmp[1][0] = in_mat.b1;
    tmp[2][0] = in_mat.c1;
    tmp[3][0] = in_mat.d1;

    tmp[0][1] = in_mat.a2;
    tmp[1][1] = in_mat.b2;
    tmp[2][1] = in_mat.c2;
    tmp[3][1] = in_mat.d2;

    tmp[0][2] = in_mat.a3;
    tmp[1][2] = in_mat.b3;
    tmp[2][2] = in_mat.c3;
    tmp[3][2] = in_mat.d3;

    tmp[0][3] = in_mat.a4;
    tmp[1][3] = in_mat.b4;
    tmp[2][3] = in_mat.c4;
    tmp[3][3] = in_mat.d4;
    return tmp;
}

void Importer::loadBones(const aiMesh* mesh)
{
    for (unsigned int i = 0; i < mesh->mNumBones; i++)
    {
        unsigned int index;
        std::string name(mesh->mBones[i]->mName.data);

        if (boneMapping.find(name) == boneMapping.end())
        {
            index = offsetMatrixes.size();
            boneMapping[name] = index;
            offsetMatrixes.emplace_back(Importer::AiToGLMMat4(mesh->mBones[i]->mOffsetMatrix));
        }
        else
        {
            index = boneMapping[name];
        }

        for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
        {
            unsigned int vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
            float weight = mesh->mBones[i]->mWeights[j].mWeight;
            vertexBoneData[vertexID].emplace_back(std::pair<unsigned int, float>({index, weight}));
        }
    }
}

void Importer::processMesh(aiMesh *mesh)
{
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        VertexPNT vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if(mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);

        vertexData.push_back(vertex);
    }
   
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    
    if(type == ModelType::RIGGED)
    {
        loadBones(mesh);
    }
}

std::vector<VertexPNT> Importer::getVertexData()
{
    return vertexData;
}

std::vector<GLuint> Importer::Importer::getIndices()
{
    return indices;
}

std::map<unsigned int, std::vector<std::pair<unsigned int, float>>> Importer::getBoneData()
{
    return vertexBoneData;
}

std::vector<glm::mat4> Importer::getOffsetMatrixes()
{
    return offsetMatrixes;
}

std::vector<glm::vec4> Importer::getWeights()
{
    std::vector<glm::vec4> weights = {};
    for(unsigned int i = 0; i < vertexBoneData.size(); i++)
    {
        std::vector<std::pair<unsigned int, float>> pairs = vertexBoneData[i];
        glm::vec4 weight = glm::vec4(0.0f);
        for(auto pair : pairs)
        {
            if(pair.first == 0)
                weight.x = pair.second;
            else if(pair.first == 1)
                weight.y = pair.second;
            else if(pair.first == 2)
                weight.z = pair.second;
        }
        weights.emplace_back(weight);
    }
    return weights;
}

Importer::~Importer()
{

}