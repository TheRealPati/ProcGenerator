#pragma once
// System Headers
#define GL_GLEXT_PROTOTYPES
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <GL/gl.h>

// Standard Headers
#include <vector>
#include <map>
#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>

//Assimp libek
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "SolidMesh.hpp"
#include "RiggedMesh.hpp"
#include "Material.hpp"

#include "Include/filesystem.hpp"

enum class ModelType
{
    PLAIN,
    RIGGED
};

class Importer
{
private:
    ModelType type;

    std::vector<VertexPNT> vertexData;
    std::vector<GLuint> indices;
	std::map<std::string, unsigned int> boneMapping;
    std::map<unsigned int, std::vector<std::pair<unsigned int, float>>> vertexBoneData;
    std::vector<glm::mat4> offsetMatrixes;

    void setup();

public:
    Importer();
    void loadModel(std::string path, ModelType type);
    void processNode(aiNode *node, const aiScene *scene);

    void processMesh(aiMesh *mesh);
    void loadBones(const aiMesh* mesh);
    static glm::mat4 AiToGLMMat4(aiMatrix4x4& in_mat);

    std::vector<VertexPNT> getVertexData();
    std::vector<GLuint> getIndices();
    std::map<unsigned int, std::vector<std::pair<unsigned int, float>>> getBoneData();
    std::vector<glm::mat4> getOffsetMatrixes();
    std::vector<glm::vec4> getWeights();

    ~Importer();
};
