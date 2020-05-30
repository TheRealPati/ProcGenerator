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

enum class KeyboardDirection 
{ 
    FORWARD, 
    BACKWARD,
    LEFT,
    RIGHT
}; 

class Camera
{
private:
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;

    float speed;

public:
    Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 up);

    void setSpeed(float);
    void move(KeyboardDirection direction, float dt);
    void rotate(float pitch, float yaw);
    glm::vec3& getPos();

    void bindUniform(GLuint shaderProgram);
    glm::mat4 getView();
    ~Camera();
};
