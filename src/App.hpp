#pragma once
// System Headers
#define GL_GLEXT_PROTOTYPES
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
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

#include "Objects/Camera.hpp"
#include "Objects/PointLight.hpp"
#include "Core/Shader.hpp"
#include "Core/Material.hpp"
#include "Core/Mesh.hpp"
#include "Core/Model.hpp"
#include "Core/GameObject.hpp"
#include "Core/UniformObjectBuffer.hpp"

#include "Include/filesystem.hpp"

enum class TerrainAlgorithm { 
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

    std::string vertexText;
    std::string fragmentText;

    std::map<std::string, Shader*> shaderPrograms;

    float deltaTime;
    float lastFrame;

    //TODO: MouseHandler
    float lastX = 300.0f;
    float lastY = 400.0f;
    float yaw = 0.0f;
    float pitch = 0.0f;

    TerrainAlgorithm terrainGenerator = TerrainAlgorithm::FAULT;
    std::vector<VertexPNT> surfaceVertexData;
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

public:
    App();
    int initWindow();
    void initShaders();

    void sceneSetup();

    void createMaterials();
    void generateTerrain();

    void bindUniforms();
    void processInput();
    void calculateDeltaTime();

    void getMouseCoords();

    void loop();
    ~App();
};

