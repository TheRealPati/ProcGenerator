
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

#include "Core/Mesh.hpp"
#include "Core/GameObject.hpp"
#include "Core/Importer.hpp"
#include "Utils.hpp"

class LodManager
{
private:
    Importer importer;

    Mesh* lowTwo;
    Mesh* lowThree;
    Mesh* lowFour;
    Mesh* highTwo;
    Mesh* highThree;
    Mesh* highFour;

    GameObject* lowTwoObj;
    GameObject* lowThreeObj;
    GameObject* lowFourObj;
    GameObject* highTwoObj;
    GameObject* highThreeObj;
    GameObject* highFourObj;

public:
    LodManager();
    void loadObjects(Material* woodMat);
    GameObject* getObject(int numOfPipe, bool highPoly);
    void setObject(std::vector<InstanceInfo> info, int numOfPipe, bool highPoly);
    ~LodManager();
};
