#version 450

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTex;
layout (location = 3) in vec4 boneIndices;
layout (location = 4) in vec4 weights;
layout (location = 5) in float modelMatIndex;

out vec2 texCoords;
out vec3 normal;
out vec3 modelPosition;

layout (std140, binding = 0) uniform VPMatrix
{
    mat4 view;
    mat4 projection;
};

struct modelMatrixes 
{
    mat4 model;
    mat4 normal;
}; 

layout(std430, binding = 4) buffer instanceBuffer
{
    modelMatrixes matrixes[];
};

void main(){
    vec4 finalPosition = projection * view * matrixes[int(modelMatIndex)].model * vec4(vertexPos, 1.0);
    modelPosition = (matrixes[int(modelMatIndex)].model * vec4(vertexPos, 1.0)).xyz;

    gl_Position = finalPosition;
    normal = normalize(matrixes[int(modelMatIndex)].normal * vec4(vertexNormal, 1.0)).xyz;
    texCoords = vertexTex;
}