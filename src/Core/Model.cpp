#include "Model.hpp"

Model::Model()
{

}

void Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        fprintf(stderr, "[ERROR] Assimp :%s!\n", importer.GetErrorString() );
        return;
    }

    //directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(createMesh(mesh));
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh* Model::createMesh(aiMesh *mesh)
{
    std::vector<VertexPNT> vertexData;
    std::vector<GLuint> indices;

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
    
    return new SolidMesh(vertexData, indices);
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