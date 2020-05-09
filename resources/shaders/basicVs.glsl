#version 450

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTex;

out vec2 texCoords;
out vec3 normal;
out vec3 modelPosition;

uniform mat4 model;
uniform mat4 normalMatrix;
layout (std140, binding = 0) uniform VPMatrix
{
    mat4 view;
    mat4 projection;
};

void main(){
    vec4 finalPosition = projection * view * model * vec4(vertexPos, 1.0);
    modelPosition = (model * vec4(vertexPos, 1.0)).xyz;

    gl_Position = finalPosition;
    normal = normalize(normalMatrix * vec4(vertexNormal, 1.0)).xyz;
    texCoords = vertexTex;
}