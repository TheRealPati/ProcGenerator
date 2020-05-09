#include "Camera.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up)
{
    this->pos = pos;
    this->front = front;
    this->up = up;
    this->speed = 1.5f;
}

void Camera::move(KeyboardDirection direction, float dt)
{
    switch(direction){
        case KeyboardDirection::FORWARD:
        pos += speed * front * dt;
        break;
        case KeyboardDirection::BACKWARD:
        pos -= speed * front * dt;
        break;
        case KeyboardDirection::LEFT:
        pos -= speed * dt *  glm::normalize(glm::cross(front, up));
        break;
        case KeyboardDirection::RIGHT:
        pos += speed * dt * glm::normalize(glm::cross(front, up));
        break;
    }
}

void Camera::setSpeed(float speed)
{
    this->speed = speed;
}

void Camera::rotate(float pitch, float yaw)
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    direction.y = sin(glm::radians(pitch));
    direction.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front = glm::normalize(direction);
}

glm::mat4 Camera::getView()
{
    return glm::lookAt(pos, pos + front, up);
}

glm::vec3& Camera::getPos()
{
    return pos;
}

Camera::~Camera()
{
    
}