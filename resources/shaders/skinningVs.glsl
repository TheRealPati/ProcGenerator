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

struct skinningMatrix 
{
    mat4 offset;
}; 

layout(std430, binding = 4) buffer instanceBuffer
{
    modelMatrixes matrixes[];
};

layout(std430, binding = 5) buffer skinningBuffer
{
    skinningMatrix skinning[];
};

void main(){

    vec4 skinnedVertexPos = 
    (skinning[int(boneIndices.x)].offset * vec4(vertexPos, 1.0)) * weights.x +
    (skinning[int(boneIndices.y)].offset * vec4(vertexPos, 1.0)) * weights.y +
    (skinning[int(boneIndices.z)].offset * vec4(vertexPos, 1.0)) * weights.z +
    (skinning[int(boneIndices.w)].offset * vec4(vertexPos, 1.0)) * weights.w;

    vec4 skinnedVertexNormal = 
    (skinning[int(boneIndices.x)].offset * vec4(vertexNormal, 1.0)) * weights.x +
    (skinning[int(boneIndices.y)].offset * vec4(vertexNormal, 1.0)) * weights.y +
    (skinning[int(boneIndices.z)].offset * vec4(vertexNormal, 1.0)) * weights.z +
    (skinning[int(boneIndices.w)].offset * vec4(vertexNormal, 1.0)) * weights.w;

    vec4 finalPosition = projection * view * matrixes[int(modelMatIndex)].model * skinnedVertexPos;
    modelPosition = (matrixes[int(modelMatIndex)].model * skinnedVertexPos).xyz;

    gl_Position = finalPosition;
    normal = normalize(matrixes[int(modelMatIndex)].normal * skinnedVertexNormal).xyz;
    texCoords = vertexTex;
}