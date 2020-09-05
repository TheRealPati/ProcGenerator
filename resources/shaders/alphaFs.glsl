#version 450

out vec4 outColor;

struct Material 
{
    sampler2D diffuse;
    vec3 specular;    
    float shininess;
}; 

in vec3 normal;
in vec2 texCoords;

uniform Material material;

void main(){
    vec4 diffuse = vec4(texture(material.diffuse, texCoords));

    if(diffuse.w <= 0.5)
        discard;

    outColor = diffuse;
}