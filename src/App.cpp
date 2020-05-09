#include "App.hpp"

App::App()
{
    glfwInit();
    deltaTime = 0.0f;
    lastFrame = 0.0f;
}

App::~App()
{
    for(auto* object : objects){
        free(object);
    }

    for(std::pair<std::string, Shader*> program : shaderPrograms){
        Shader* shader = program.second;
        glDeleteProgram(shader->getID());
    }

    fprintf(stderr, "[INFO] Clean up done!\n");
    
    glfwTerminate();
}

void debugMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                  const GLchar *message, const void *userParam)
{
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

int App::initWindow()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //TODO
    window = glfwCreateWindow(windowWidth, windowHeight, "Procedurális Generátor BME Önlab - Patiii :3", nullptr, nullptr);

    // Context check
    if (window == nullptr) {
        fprintf(stderr, "[ERROR] Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create context and load OpenGL functions
    glfwMakeContextCurrent(window);
    fprintf(stderr, "[INFO] OpenGL %s\n", glGetString(GL_VERSION));

    // GLEW initializing functions
    glewExperimental = GL_TRUE;
    if(glewInit()){
        fprintf(stderr, "[ERROR] GLEW was not initialized successfully!");
        return EXIT_FAILURE;
    }

    // enable OpenGL debug context if context allows for debug context
    int flags; 
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
        glDebugMessageCallback(debugMessage, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEBUG_OUTPUT);
    //glEnable(GL_CULL_FACE);  
    //glCullFace(GL_FRONT);  
    //glFrontFace(GL_CCW);
    return EXIT_SUCCESS;
}

void App::createMaterials()
{
    //Creating mats
    fprintf(stderr, "[INFO] Texture files opening...\n");
    Material* surfaceMat = new Material(shaderPrograms.at("basic"));
    surfaceMat->loadTexture(FileSystem::getPath("resources/textures/dirt.jpg"));
    surfaceMat->setMaterialProps(glm::vec3(0.2f), 32);
    materials.insert(std::pair<std::string, Material*>("surface", surfaceMat));
    fprintf(stderr, "[INFO] Materials loaded!\n");
}

void App::generateTerrain()
{
    //Gen algorithm
    TerrainGenAlgo* generator;
    int surfaceSideSize = 257;
    //int surfaceSideSize = 50;
    int density = 4;
    if(terrainGenerator == TerrainAlgorithm::PLAIN)
        generator = new PlainGen(surfaceSideSize, density);
    else if(terrainGenerator == TerrainAlgorithm::FAULT)
        generator = new FaultGen(surfaceSideSize, density);
    else if(terrainGenerator == TerrainAlgorithm::SINE)
        generator = new FaultGenSine(surfaceSideSize, density);
    else if(terrainGenerator == TerrainAlgorithm::CIRCLE)
        generator = new CircleGen(surfaceSideSize, density);
    else if(terrainGenerator == TerrainAlgorithm::MDP_NON_WRAP)
        generator = new MDPNonWrapping(surfaceSideSize, density);
    else if(terrainGenerator == TerrainAlgorithm::MDP_WRAP)
        generator = new MDPWrapping(surfaceSideSize, density);

    fprintf(stderr, "[INFO] Surface creating...\n");
    surfaceVertexData = generator->generateVertices();
    std::vector<GLuint> indices = generator->generateIndices();
    // Creating our surface
    Mesh* surfaceMesh = new Mesh(surfaceVertexData, indices);
    Model* surfaceModel = new Model();
    surfaceModel->addMesh(surfaceMesh);
    surfaceModel->addMat(materials["surface"], 0);

    GameObject* surfaceObject = new GameObject();
    surfaceObject->setModel(surfaceModel);
    fprintf(stderr, "[INFO] Surface created!\n");

    objects.push_back(surfaceObject);
}

void App::sceneSetup()
{
    camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(45.0f), 3.0f/4.0f, 0.1f, 100.0f);

    createMaterials();
    generateTerrain();

    dirLight = new DirectionalLight(glm::vec3(10.0f, -10.0f, 0.0f));
    dirLight->setLightColor(glm::vec3(0.05f), glm::vec3(0.3f), glm::vec3(0.8f));

    //TODO surfaceVertexDatából venni és néhány pontra odahelyezni
    lights[0] = new PointLight(glm::vec3(0.0f, 0.0f, 20.0f));
    lights[1] = new PointLight(glm::vec3(0.0f, 0.0f, 0.0f));
    lights[2] = new PointLight(glm::vec3(20.0f, 0.0f, 20.0f));
    lights[3] = new PointLight(glm::vec3(0.0f, 0.0f, 0.0f));

    for(int i=0; i<4; i++){
        lights[i]->setLightColor(glm::vec3(1.0f), glm::vec3(0.2f), glm::vec3(0.8f));
        lights[i]->setLightIntensity(1.0f, 0.35, 0.44f);
        //7 distance 0.7 1.8
        //13 distance 0.35 0.44
        //32 distance 0.14 0.07
        //50 distance 0.09 0.032
    }

    spotLight = new SpotLight(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 10.0f, 0.0f));
    spotLight->setLightColor(glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f));
    spotLight->setLightIntensity(1.0f, 0.9f, 0.032f);
    spotLight->setSpotArea(glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(32.5f)));

    //UBOS
    matrixUbo = new UniformBufferObject(0);
    matrixUbo->preserveVPMatrix();
    matrixUbo->setVPmatrix(camera->getView(), projection);
    pointsUbo = new UniformBufferObject(1);
    pointsUbo->preservePointLight(4);
    for(int i=0; i<4; i++){
        pointsUbo->setPointLights(lights[i], i);
    }
    directionUbo = new UniformBufferObject(2);
    directionUbo->preserveDirectionalLight();
    directionUbo->setDirectionLights(dirLight);
    spotUbo = new UniformBufferObject(3);
    spotUbo->preserveSpotLight();
    spotUbo->setSpotLights(spotLight);
    
    fprintf(stderr, "[INFO] Objects loaded!\n");

}

void App::bindUniforms()
{
    matrixUbo->setVPmatrix(camera->getView(), projection);
}

void App::initShaders()
{
    std::string basicVS = FileSystem::getPath("resources/shaders/basicVs.glsl");
    std::string basicFS = FileSystem::getPath("resources/shaders/basicFs.glsl");
    Shader* basicShader = new Shader(basicVS, basicFS);
    shaderPrograms.insert(std::pair<std::string, Shader*>("basic", basicShader));
}

void App::processInput() {
    camera->rotate(pitch, yaw);

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->move(KeyboardDirection::FORWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->move(KeyboardDirection::BACKWARD, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->move(KeyboardDirection::LEFT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->move(KeyboardDirection::RIGHT, deltaTime);

    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->setSpeed(8.0f);
    else
        camera->setSpeed(2.0f);
}

void App::calculateDeltaTime() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void App::loop()
{
    // Rendering Loop
    while (glfwWindowShouldClose(window) == false) {
        calculateDeltaTime();
        getMouseCoords();
        processInput();
        
        // Background Fill Color
        glClearColor(0.75f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->bindUniforms();

        for(auto* object : objects){
            object->draw(camera);
        }

        // Flip Buffers and Draw
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void App::getMouseCoords(){
    GLdouble xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    float sensitivity = 0.05f;
    float xoffset = (xPos - lastX) * sensitivity;
    float yoffset = (lastY - yPos) * sensitivity;

    lastX = xPos;
    lastY = yPos;

    pitch += yoffset;
    yaw += xoffset;

    //Y restriction
    if(pitch > 89.0f)
    pitch = 89.0f;
    if(pitch < -89.0f)
    pitch = -89.0f;
}