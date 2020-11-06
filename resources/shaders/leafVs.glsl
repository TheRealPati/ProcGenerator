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

uniform vec3 camera;

vec3 ProjectToPlane(vec3 Point, vec3 PlanePoint, vec3 PlaneNormal)
{
    vec3 v = Point - PlanePoint;
    float Len = dot(v, PlaneNormal);
    vec3 d = Len * PlaneNormal;
    return (Point - d);
} 

mat4 rotationMatrix(vec3 axis, float angle) {
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

vec3 rotate(vec3 v, vec3 axis, float angle) {
	mat4 m = rotationMatrix(axis, angle);
	return (m * vec4(v, 1.0)).xyz;
}

void main(){

    mat4 modelMat = matrixes[gl_InstanceID].model;
    modelPosition = (modelMat * vec4(vertexPos, 1.0)).xyz;

    vec3 distance = modelPosition - camera;
    vec3 transformedNormal = normalize((transpose(inverse(modelMat)) * vec4(vertexNormal, 1.0)).xyz);
    vec3 directionAxis = normalize((transpose(inverse(modelMat)) * vec4(0.0, 1.0, 0.0, 1.0)).xyz);
    vec3 projectedDist = normalize(ProjectToPlane(distance, modelPosition, directionAxis));

    //float angle = atan(transformedNormal.x  * projectedDist.z - projectedDist.x * transformedNormal.z,  transformedNormal.x * projectedDist.x + transformedNormal.z * projectedDist.z);
    float dotProd = transformedNormal.x * projectedDist.x + transformedNormal.y * projectedDist.y + transformedNormal.z * projectedDist.z;
    float det = 
    transformedNormal.x * projectedDist.y * directionAxis.z 
    + projectedDist.x * directionAxis.y * transformedNormal.z 
    + directionAxis.x * transformedNormal.y * projectedDist.z 
    - transformedNormal.z * projectedDist.y * directionAxis.x 
    - projectedDist.z * directionAxis.y * transformedNormal.x 
    - directionAxis.z * transformedNormal.y * projectedDist.x;
    float angle = atan(det, dotProd);

    vec4 finalPosition = projection * view * modelMat * rotationMatrix(directionAxis, -angle) * vec4(vertexPos, 1.0);

    gl_Position = finalPosition;
    normal = (rotationMatrix(directionAxis, angle) * vec4(transformedNormal, 1.0)).xyz;
    texCoords = vertexTex;
}