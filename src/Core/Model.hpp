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

class Model
{
private:
    GLuint VAO;
    std::vector<Mesh*> meshes;
    std::vector<Material*> materials;
    std::map<GLuint, GLuint> meshMatMap;

    void setup();

public:
    Model();
    void addMesh(Mesh* mesh);
    void addMat(Material* mat, GLuint index);

    void draw(glm::mat4& model, glm::mat4& normal, glm::vec3& cameraPos, int count);
    ~Model();
};
