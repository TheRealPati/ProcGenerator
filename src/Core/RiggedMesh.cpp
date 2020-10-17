#include "RiggedMesh.hpp"

RiggedMesh::RiggedMesh(std::vector<VertexPNT> vertexData, std::vector<GLuint> indices, std::vector<glm::vec4> skinning, std::vector<InstanceInfo> instanceData)
{
    this->vertexData = vertexData;
    this->indices = indices;
    this->instanceData = instanceData;
    this->skinning = skinning;

    setup();
}

void RiggedMesh::setInstanceInfo(std::vector<InstanceInfo> instanceData)
{
    this->instanceData = instanceData;

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); 
    glBufferData(GL_ARRAY_BUFFER, instanceData.size() * sizeof(InstanceInfo), &instanceData[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(3); 
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceInfo), (void*)offsetof(InstanceInfo, boneIndices));
    glVertexAttribDivisor(3, 1);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceInfo), (void*)offsetof(InstanceInfo, modelMatIndex));
    glVertexAttribDivisor(5, 1);

}

void RiggedMesh::setup()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &instanceVBO);
    glGenBuffers(1, &skinningVBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(VertexPNT), &vertexData[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (void*)offsetof(VertexPNT, position));
    glEnableVertexAttribArray(1);  
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (void*)offsetof(VertexPNT, normal));
    glEnableVertexAttribArray(2);  
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPNT), (void*)offsetof(VertexPNT, texCoords));

    //Skinning buffer
    glBindBuffer(GL_ARRAY_BUFFER, skinningVBO); 
    glBufferData(GL_ARRAY_BUFFER, skinning.size() * sizeof(glm::vec4), &skinning[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0);

    //Instance buffer
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); 
    glBufferData(GL_ARRAY_BUFFER, instanceData.size() * sizeof(InstanceInfo), &instanceData[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(3); 
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceInfo), (void*)offsetof(InstanceInfo, boneIndices));
    glVertexAttribDivisor(3, 1);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceInfo), (void*)offsetof(InstanceInfo, modelMatIndex));
    glVertexAttribDivisor(5, 1);

    //EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    //Instance data
    //glEnableVertexAttribArray(2);
    //glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
    //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

    glBindVertexArray(0);
}

void RiggedMesh::draw(int count){
    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, count);
    glBindVertexArray(0);
}

RiggedMesh::~RiggedMesh(){
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}