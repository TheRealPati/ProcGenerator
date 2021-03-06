#include "App.hpp"

App::App(Randomizer& randomizer) : randomizer(randomizer)
{
    glfwInit();
    deltaTime = 0.0f;
    lastFrame = 0.0f;
}

App::~App()
{
    glfwTerminate();

    //Lights
    delete dirLight;
    delete lights[0];
    delete lights[1];
    delete lights[2];
    delete lights[3];
    delete spotLight;

    //UBO
    delete matrixUbo;
    delete pointsUbo;
    delete directionUbo;
    delete spotUbo;

    //SSBO
    delete modelMatrixes;
    delete riggingMatrixes;
    delete groundBillboardMatrixes;
    delete leafBillboardMatrixes;

    delete grass;
    delete forest;
    delete lodManager;
    delete scatterer;
    delete camera;

    for(GameObject* object : objects){
        delete object;
    }

    for(std::pair<std::string, Material*> matPair : materials){
        Material* mat = matPair.second;
        delete mat;
    }

    for(std::pair<std::string, Shader*> program : shaderPrograms){
        Shader* shader = program.second;
        delete shader;
    }

    fprintf(stderr, "[INFO] Clean up done!\n");
    
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

    // Enable OpenGL debug context if context allows for debug context
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
    Material* surfaceMat = new MaterialGroup(shaderPrograms.at("ground"));
    surfaceMat->generateTexture(256, 256);
    surfaceMat->loadTexture(FileSystem::getPath("resources/textures/dirt.jpg"), GL_RGB); //1
    surfaceMat->loadTexture(FileSystem::getPath("resources/textures/grass.jpg"), GL_RGB); //2
    surfaceMat->loadTexture(FileSystem::getPath("resources/textures/dirtrock.jpg"), GL_RGB); //3
    surfaceMat->loadTexture(FileSystem::getPath("resources/textures/rock.jpg"), GL_RGB); //4
    surfaceMat->loadTexture(FileSystem::getPath("resources/textures/water.jpg"), GL_RGB); //5
    surfaceMat->loadTexture(FileSystem::getPath("resources/textures/snow.jpg"), GL_RGB); //6
    surfaceMat->setMaterialProps(glm::vec3(0.2f), 32);
    materials.insert(std::pair<std::string, Material*>("surface", surfaceMat));

    Material* perlinMat = new MaterialSingle(shaderPrograms.at("basic"));
    perlinMat->generateTexture(256, 256);
    perlinMat->setMaterialProps(glm::vec3(0.02f), 32);
    materials.insert(std::pair<std::string, Material*>("perlin", perlinMat));

    Material* dimwavesMat = new MaterialSingle(shaderPrograms.at("rigging"));
    dimwavesMat->loadTexture(FileSystem::getPath("resources/textures/dimensionwaves.jpg"), GL_RGB);
    dimwavesMat->setMaterialProps(glm::vec3(0.2f), 16);
    materials.insert(std::pair<std::string, Material*>("dimensionwaves", dimwavesMat));

    Material* woodMat = new MaterialSingle(shaderPrograms.at("rigging"));
    woodMat->loadTexture(FileSystem::getPath("resources/textures/wood.jpg"), GL_RGB);
    //woodMat->generateTexture(256, 256);
    woodMat->setMaterialProps(glm::vec3(0.004f), 256);
    materials.insert(std::pair<std::string, Material*>("wood", woodMat));

    Material* weedMat = new MaterialSingle(shaderPrograms.at("alphaTest"));
    weedMat->loadTexture(FileSystem::getPath("resources/textures/weed.png"), GL_RGBA);
    weedMat->setMaterialProps(glm::vec3(0.02f), 16);
    materials.insert(std::pair<std::string, Material*>("weed", weedMat));

    Material* leafMat = new MaterialSingle(shaderPrograms.at("leaf"));
    leafMat->loadTexture(FileSystem::getPath("resources/textures/leaf.png"), GL_RGBA);
    leafMat->setMaterialProps(glm::vec3(0.02f), 16);
    materials.insert(std::pair<std::string, Material*>("leaf", leafMat));
}

void App::generateTerrain()
{
    //Gen algorithm
    TerrainGenAlgo* generator;
    scatterer = new ObjectScatterer(randomizer);
    int surfaceSideSize = 1025;
    //int surfaceSideSize = 50;
    int density = 4;
    maxSideSize = surfaceSideSize / density;

    if(terrainGenerator == TerrainAlgorithm::PLAIN)
        generator = new PlainGen(surfaceSideSize, density, randomizer);
    else if(terrainGenerator == TerrainAlgorithm::FAULT)
        generator = new FaultGen(surfaceSideSize, density, randomizer);
    else if(terrainGenerator == TerrainAlgorithm::SINE)
        generator = new FaultGenSine(surfaceSideSize, density, randomizer);
    else if(terrainGenerator == TerrainAlgorithm::CIRCLE)
        generator = new CircleGen(surfaceSideSize, density, randomizer);
    else if(terrainGenerator == TerrainAlgorithm::MDP_NON_WRAP)
        generator = new MDPNonWrapping(surfaceSideSize, density, randomizer);
    else if(terrainGenerator == TerrainAlgorithm::MDP_WRAP)
        generator = new MDPWrapping(surfaceSideSize, density, randomizer);

    std::vector<VertexPNT> surfaceVertexData = generator->generateVertices();
    std::vector<GLuint> indices = generator->generateIndices();
    //Feeding scatterer with surfaceData
    scatterer->feedSurfaceData(surfaceVertexData, maxSideSize);
    // Creating our surface
    Mesh* surfaceMesh = new SolidMesh(surfaceVertexData, indices);
    Model* surfaceModel = new Model();
    surfaceModel->addMesh(surfaceMesh);
    surfaceModel->addMat(materials["surface"], 0);

    GameObject* surfaceObject = new GameObject();
    surfaceObject->setModel(surfaceModel);

    delete generator;

    objects.push_back(surfaceObject);
}

void App::generateTrees()
{
    forest = new Forest(randomizer, scatterer, 30);
    forest->populate(maxSideSize, places, rigging, leafBillboard);
}

void App::generateBranchObjects(){
    lodManager = new LodManager();
    lodManager->loadObjects(materials["wood"]);

    //Low poly
    objects.push_back(lodManager->getObject(2, true));
    objects.push_back(lodManager->getObject(3, true));
    objects.push_back(lodManager->getObject(4, true));
    //High poly
    objects.push_back(lodManager->getObject(2, false));
    objects.push_back(lodManager->getObject(3, false));
    objects.push_back(lodManager->getObject(4, false));
}

void App::generateGroundFoliage()
{
    grass = new Grassfield(randomizer, scatterer);
    grass->populate(maxSideSize, groundBillboard, 8000);

    CrossedPlane plane = CrossedPlane(1.0f);

    Mesh* grassMesh = new InstancedMesh(plane.getVertexData(), plane.getIndexData());

    Model* grassModel = new Model();
    grassModel->addMesh(grassMesh);
    grassModel->addMat(materials["weed"], 0);

    GameObject* grassObject = new GameObject();

    grassObject->setInstanceCount(groundBillboard.size());
    grassObject->setModel(grassModel);

    objects.push_back(grassObject);
}

void App::generateLeafFoliage()
{
    Plane plane = Plane(1.3f);

    Mesh* leafMesh = new InstancedMesh(plane.getVertexData(), plane.getIndexData());

    Model* leafModel = new Model();
    leafModel->addMesh(leafMesh);
    leafModel->addMat(materials["leaf"], 0);

    GameObject* leafObject = new GameObject();

    leafObject->setInstanceCount(leafBillboard.size());
    leafObject->setModel(leafModel);

    objects.push_back(leafObject);
}

void App::sceneSetup()
{
    camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 300.0f);

    fprintf(stderr, "[INFO] Texture files opening...\n");
    createMaterials();
    fprintf(stderr, "[INFO] Materials loaded!\n");

    fprintf(stderr, "[INFO] Surface creating...\n");
    generateTerrain();
    fprintf(stderr, "[INFO] Surface created!\n");

    fprintf(stderr, "[INFO] Positioning objects...\n");
    generateTrees();
    generateGroundFoliage();
    generateLeafFoliage();
    generateBranchObjects();

    fprintf(stderr, "[INFO] Objects loaded!\n");

    fprintf(stderr, "[INFO] Making some light!\n");
    dirLight = new DirectionalLight(glm::vec3(10.0f, -10.0f, 0.0f));
    dirLight->setLightColor(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f));

    lights[0] = new PointLight(glm::vec3(10.0f, scatterer->getHeight(10.0f, 10.0f)+2.0f, 10.0f));
    lights[1] = new PointLight(glm::vec3(20.0f, scatterer->getHeight(20.0f, 20.0f)+2.0f, 20.0f));
    lights[2] = new PointLight(glm::vec3(30.0f, scatterer->getHeight(30.0f, 30.0f)+2.0f, 30.0f));
    lights[3] = new PointLight(glm::vec3(48.0f, scatterer->getHeight(40.0f, 40.0f)+2.0f, 40.0f));

    for(int i=0; i<4; i++)
    {
        lights[i]->setLightColor(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
        lights[i]->setLightIntensity(1.0f, 0.7, 1.8f);
        //Mostani 0.7 0.17
        //7 distance 0.7 1.8
        //13 distance 0.35 0.44
        //32 distance 0.14 0.07
        //50 distance 0.09 0.032
    }

    spotLight = new SpotLight(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 10.0f, 0.0f));
    spotLight->setLightColor(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
    spotLight->setLightIntensity(1.0f, 0.9f, 0.032f);
    spotLight->setSpotArea(glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(32.5f)));

    //UBOS
    matrixUbo = new UniformBufferObject(0);
    matrixUbo->preserveVPMatrix();
    matrixUbo->setVPmatrix(camera->getView(), projection);
    pointsUbo = new UniformBufferObject(1);
    pointsUbo->preservePointLight(4);
    for(int i=0; i<4; i++)
    {
        pointsUbo->setPointLights(lights[i], i);
    }
    directionUbo = new UniformBufferObject(2);
    directionUbo->preserveDirectionalLight();
    directionUbo->setDirectionLights(dirLight);
    spotUbo = new UniformBufferObject(3);
    spotUbo->preserveSpotLight();
    spotUbo->setSpotLights(spotLight);
    
    fprintf(stderr, "[INFO] Lights functioning!\n");

    fprintf(stderr, "[INFO] Be prepared. Only some last magic left!\n");

    //SSBO
    modelMatrixes = new ShaderStorageBuffer(4);
    riggingMatrixes = new ShaderStorageBuffer(5);
    groundBillboardMatrixes = new ShaderStorageBuffer(6);
    leafBillboardMatrixes = new ShaderStorageBuffer(7);
    modelMatrixes->preserveMat(places.size(), 2);
    riggingMatrixes->preserveMat(rigging.size(), 1);
    groundBillboardMatrixes->preserveMat(groundBillboard.size(), 1);
    leafBillboardMatrixes->preserveMat(leafBillboard.size(), 1);

    for(unsigned int i = 0; i < places.size(); i++)
    {
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(places[i]));
        modelMatrixes->setModelMat(places[i], normalMatrix, i);
    }
    for(unsigned int i = 0; i < rigging.size(); i++)
    {
        riggingMatrixes->setMat(rigging[i], i);
    }
    for(unsigned int i = 0; i < groundBillboard.size(); i++)
    {
        groundBillboardMatrixes->setMat(groundBillboard[i], i);
    }
    for(unsigned int i = 0; i < leafBillboard.size(); i++)
    {
        leafBillboardMatrixes->setMat(leafBillboard[i], i);
    }

    fprintf(stderr, "[INFO] Okay, okay, should be ready.\n");

}

void App::bindUniforms()
{
    matrixUbo->setVPmatrix(camera->getView(), projection);
}

void App::setModelDetails()
{
    std::vector<InstanceInfo> small;
    std::vector<InstanceInfo> big;

    //2 way objects
    for(InstanceInfo i : forest->getInstanceInfo("twoWay"))
    {
        if(glm::length(places[i.modelMatIndex][3].xyz() - camera->getPos()) < lodDistance)
            small.emplace_back(i);
        else
            big.emplace_back(i);
    }
    lodManager->setObject(small, 2, false);
    lodManager->setObject(big, 2, true);
    small.clear();
    big.clear();
    //3 way objects
    for(InstanceInfo i : forest->getInstanceInfo("threeWay"))
    {
        if(glm::length(places[i.modelMatIndex][3].xyz() - camera->getPos()) < lodDistance)
            small.emplace_back(i);
        else
            big.emplace_back(i);
    }
    lodManager->setObject(small, 3, false);
    lodManager->setObject(big, 3, true);
    small.clear();
    big.clear();

    //3 way objects
    for(InstanceInfo i : forest->getInstanceInfo("fourWay"))
    {
        if(glm::length(places[i.modelMatIndex][3].xyz() - camera->getPos()) < lodDistance)
            small.emplace_back(i);
        else
            big.emplace_back(i);
    }
    lodManager->setObject(small, 4, false);
    lodManager->setObject(big, 4, true);
    small.clear();
    big.clear();
}

void App::initShaders()
{
    //3
    std::string basicVS = FileSystem::getPath("resources/shaders/basicVs.glsl");
    std::string basicFS = FileSystem::getPath("resources/shaders/basicFs.glsl");
    Shader* basicShader = new Shader(basicVS, basicFS);
    shaderPrograms.insert(std::pair<std::string, Shader*>("basic", basicShader));

    //6
    std::string riggingVS = FileSystem::getPath("resources/shaders/riggingVs.glsl");
    Shader* riggingShader = new Shader(riggingVS, basicFS);
    shaderPrograms.insert(std::pair<std::string, Shader*>("rigging", riggingShader));

    //9
    std::string alphaVS = FileSystem::getPath("resources/shaders/alphaVs.glsl");
    std::string alphaFS = FileSystem::getPath("resources/shaders/alphaFs.glsl");
    Shader* alphaShader = new Shader(alphaVS, alphaFS);
    shaderPrograms.insert(std::pair<std::string, Shader*>("alphaTest", alphaShader));

    //12
    std::string leafVs = FileSystem::getPath("resources/shaders/leafVs.glsl");
    Shader* leafShader = new Shader(leafVs, alphaFS);
    shaderPrograms.insert(std::pair<std::string, Shader*>("leaf", leafShader));

    //15
    std::string groundFS = FileSystem::getPath("resources/shaders/groundFs.glsl");
    Shader* groundShader = new Shader(basicVS, groundFS);
    shaderPrograms.insert(std::pair<std::string, Shader*>("ground", groundShader));
}

void App::processInput() 
{
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
        camera->setSpeed(30.0f);
    else
        camera->setSpeed(2.0f);
}

void App::calculateDeltaTime() 
{
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
        
        glClearColor(0.75f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->bindUniforms();
        this->setModelDetails();

        for(auto* object : objects)
        {
            object->draw(camera);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void App::getMouseCoords()
{
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