#version 450

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTex;

out vec2 texCoords;
out vec3 normal;
out vec3 modelPosition;

layout (std140, binding = 0) uniform VPMatrix
{
    mat4 view;
    mat4 projection;
};

struct billboardMatrix 
{
    mat4 model;
}; 

layout(std430, binding = 7) buffer billboardBuffer
{
    billboardMatrix matrixes[];
};

void main(){

    vec4 finalPosition = projection * view * matrixes[gl_InstanceID].model * vec4(vertexPos, 1.0);
    modelPosition = (matrixes[gl_InstanceID].model * vec4(vertexPos, 1.0)).xyz;

    gl_Position = finalPosition;
    normal = vertexNormal;
    texCoords = vertexTex;
}