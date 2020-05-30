#pragma once
// System Headers
#define GL_GLEXT_PROTOTYPES
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <GL/gl.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <map>
#include <utility>
#include <fstream>
#include <sstream>

// App Headers
#include "TerrainGenerator/PlainGen.hpp"
#include "TerrainGenerator/FaultGen.hpp"
#include "TerrainGenerator/FaultGenSine.hpp"
#include "TerrainGenerator/CircleGen.hpp"
#include "TerrainGenerator/MDPNonWrapping.hpp"
#include "TerrainGenerator/MDPWrapping.hpp"
#include "TerrainGenerator/ObjectScatterer.hpp"

#include "Objects/Camera.hpp"
#include "Objects/PointLight.hpp"
#include "Objects/Forest.hpp"
#include "Core/Shader.hpp"
#include "Core/Material.hpp"
#include "Core/Mesh.hpp"
#include "Core/SolidMesh.hpp"
#include "Core/RiggedMesh.hpp"
#include "Core/Model.hpp"
#include "Core/GameObject.hpp"
#include "Core/UniformObjectBuffer.hpp"
#include "Core/ShaderStorageBuffer.hpp"

#include "Primitives/Cube.hpp"
#include "Primitives/Cylinder.hpp"

#include "Include/filesystem.hpp"

enum class TerrainAlgorithm 
{ 
    PLAIN,
    FAULT,
    SINE,
    CIRCLE,
    MDP_NON_WRAP,
    MDP_WRAP,
}; 

class App
{
private:
    const int windowWidth = 800;
    const int windowHeight = 600;

    Randomizer& randomizer;

    std::map<std::string, Shader*> shaderPrograms;

    float deltaTime;
    float lastFrame;

    //TODO: MouseHandler
    float lastX = 300.0f;
    float lastY = 400.0f;
    float yaw = 0.0f;
    float pitch = 0.0f;

    TerrainAlgorithm terrainGenerator = TerrainAlgorithm::MDP_WRAP;
    float maxSideSize;
    ObjectScatterer* scatterer;
    std::vector<glm::mat4> places;
    std::vector<glm::mat4> skinning = { glm::mat4(1.0f) };
    glm::mat4 projection;

    GLFWwindow* window;

    std::vector<GameObject*> objects;
    std::map<std::string, Material*> materials;
    Camera* camera;

    PointLight* lights[4];
    DirectionalLight* dirLight;
    SpotLight* spotLight;

    UniformBufferObject* matrixUbo;
    UniformBufferObject* pointsUbo;
    UniformBufferObject* directionUbo;
    UniformBufferObject* spotUbo;
    ShaderStorageBuffer* modelMatrixes;
    ShaderStorageBuffer* skinningMatrixes;

public:
    App(Randomizer& randomizer);
    int initWindow();
    void initShaders();

    void sceneSetup();

    void createMaterials();
    void generateTerrain();
    void generateTrees();
    void generateEntities();

    void bindUniforms();
    void processInput();
    void calculateDeltaTime();

    void getMouseCoords();

    void loop();
    ~App();
};


