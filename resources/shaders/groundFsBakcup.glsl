#version 450

out vec4 outColor;

struct Material 
{
    vec3 specular;    
    float shininess;
}; 

struct PointLight 
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

layout (std140, binding = 1) uniform PointLights
{
    PointLight pointLight[4];
};

struct DirectionLight 
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout (std140, binding = 2) uniform DirectionLights
{
    DirectionLight directionLight;
};

struct SpotLight 
{
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};

layout (std140, binding = 3) uniform SpotLights
{
    SpotLight spotLight;
};

struct Camera 
{
    vec3 position;
};

in vec3 normal;
in vec3 modelPosition;
in vec2 texCoords;

uniform Material material;
uniform Camera camera;

vec3 calcDirectionLight(DirectionLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);
vec3 calcTexFromHeight();

void main(){
    vec3 viewDir = normalize(camera.position - modelPosition);
    
    vec3 result = calcDirectionLight(directionLight, normal, viewDir);
    for(int i = 0; i < 4; i++)
    {
        result += calcPointLight(pointLight[i], normal, viewDir);
    }
    result += calcSpotLight(spotLight, normal, viewDir);

    outColor = vec4(result, 1.0);
}

vec3 calcDirectionLight(DirectionLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 texPoint = calcTexFromHeight();
    vec3 ambient = light.ambient * texPoint;
    vec3 diffuse = light.diffuse * diff * texPoint;
    vec3 specular = light.specular * (spec * material.specular);
    return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - modelPosition);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - modelPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    vec3 texPoint = calcTexFromHeight();
    vec3 ambient = light.ambient * texPoint;
    vec3 diffuse = light.diffuse * diff * texPoint;
    vec3 specular = light.specular * (spec * material.specular);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - modelPosition);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float distance = length(light.position - modelPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 texPoint = calcTexFromHeight();
    vec3 ambient = light.ambient * texPoint;
    vec3 diffuse = light.diffuse * diff * texPoint;
    vec3 specular = light.specular * (spec * material.specular);

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

vec3 calcTexFromHeight()
{
        return vec3(0.3f, 0.4f, 0.6f);
}