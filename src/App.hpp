#pragma once
// System Headers
#define GL_GLEXT_PROTOTYPES
#define GLM_FORCE_SWIZZLE
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
#include <cmath>

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
#include "Objects/LodManager.hpp"
#include "Objects/Forest.hpp"
#include "Objects/Grassfield.hpp"

#include "Core/Shader.hpp"
#include "Core/Material.hpp"
#include "Core/MaterialSingle.hpp"
#include "Core/MaterialGroup.hpp"

#include "Core/Mesh.hpp"
#include "Core/SolidMesh.hpp"
#include "Core/RiggedMesh.hpp"
#include "Core/InstancedMesh.hpp"
#include "Core/Model.hpp"
#include "Core/GameObject.hpp"

#include "Core/UniformObjectBuffer.hpp"
#include "Core/ShaderStorageBuffer.hpp"

#include "Primitives/Cube.hpp"
#include "Primitives/Cylinder.hpp"
#include "Primitives/CrossedPlane.hpp"

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
    const int windowWidth = 1280;
    const int windowHeight = 720;

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

    Importer importer;
    ObjectScatterer* scatterer;
    std::vector<glm::mat4> places;
    std::vector<glm::mat4> skinning = { glm::mat4(1.0f) };
    std::vector<glm::mat4> groundBillboard;
    std::vector<glm::mat4> leafBillboard;
    glm::mat4 projection;

    GLFWwindow* window;

    std::vector<GameObject*> objects;
    std::map<std::string, Material*> materials;
    Camera* camera;
    Forest* forest;
    Grassfield* grass;
    LodManager* lodManager;
    const float lodDistance = 50.0f;

    PointLight* lights[4];
    DirectionalLight* dirLight;
    SpotLight* spotLight;

    UniformBufferObject* matrixUbo;
    UniformBufferObject* pointsUbo;
    UniformBufferObject* directionUbo;
    UniformBufferObject* spotUbo;
    ShaderStorageBuffer* modelMatrixes;
    ShaderStorageBuffer* skinningMatrixes;
    ShaderStorageBuffer* groundBillboardMatrixes;
    ShaderStorageBuffer* leafBillboardMatrixes;

public:
    App(Randomizer& randomizer);
    int initWindow();
    void initShaders();

    void sceneSetup();

    void createMaterials();
    void generateTerrain();
    void generateTrees();
    void generateGroundFoliage();
    void generateLeafFoliage();
    void generateBranchObjects();

    void bindUniforms();
    void setModelDetails();
    void processInput();
    void calculateDeltaTime();

    void getMouseCoords();

    void loop();
    ~App();
};


