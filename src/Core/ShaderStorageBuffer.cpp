#include "ShaderStorageBuffer.hpp"

ShaderStorageBuffer::ShaderStorageBuffer(GLuint bindingPoint)
{
    this->bindingPoint = bindingPoint;
    glGenBuffers(1, &SSBO);
}

void ShaderStorageBuffer::preserveMat(int countOfMatrix, int numOfMatrixEach)
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, countOfMatrix * numOfMatrixEach * sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, bindingPoint, SSBO, 0, countOfMatrix * numOfMatrixEach * sizeof(glm::mat4));
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ShaderStorageBuffer::setModelMat(glm::mat4& matrix, glm::mat4& normalMatrix, int index)
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, index * 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(matrix));
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, index * 2 * sizeof(glm::mat4) + sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(normalMatrix));
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ShaderStorageBuffer::setSkinningMat(glm::mat4& matrix, int index)
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(matrix));
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}