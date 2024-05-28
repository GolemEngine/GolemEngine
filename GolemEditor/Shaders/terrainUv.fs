#version 460 core

struct PointLight
{
    vec3 position;
    
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirLight
{
    vec3 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;
  
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    
    float constant;
    float linear;
    float quadratic; 
};

uniform sampler2D ourTexture;
uniform bool useTexture;
uniform int nbrDirLights;
uniform int nbrPointLights;
uniform int nbrSpotLights;
uniform DirLight dirLights[3];
uniform PointLight pointLights[10];
uniform SpotLight spotLights[10];
vec4 ProcessDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec4 ProcessPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 ProcessSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

in vec2 TexCoord;

out vec2 FragColor;


void main()
{
    FragColor = TexCoord;
}