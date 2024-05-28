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

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform sampler2D ourTexture3;
uniform float texture1Level;
uniform float texture2Level;
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

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec2 terrainPos;

in float minY;    // Input from vertex shader
in float maxY;    // Input from vertex shader

layout(location = 0) out vec4 FragColor;
layout(location = 2) out vec2 FragUv;


void main()
{
    FragUv = terrainPos;
    if (useTexture)
    {
        vec3 viewDir = normalize(-FragPos);
    
        vec4 light = vec4(0);
    
        for (int i = 0; i < nbrDirLights; i++)
            light += ProcessDirLight(dirLights[i], Normal, viewDir);
    
        for (int i = 0; i < nbrPointLights; i++)
            light += ProcessPointLight(pointLights[i], Normal, FragPos, viewDir);
    
        for (int i = 0; i < nbrSpotLights; i++)
            light += ProcessSpotLight(spotLights[i], Normal, FragPos, viewDir);
        
        // Set the texure
        vec4 tex1 = texture(ourTexture1, TexCoord * 10.0);
        vec4 tex2 = texture(ourTexture2, TexCoord * 10.0);
        vec4 tex3 = texture(ourTexture3, TexCoord * 10.0);

        // Determine which texture to use based on height
        vec4 finalTexture;
        if (FragPos.y < texture1Level) {
            finalTexture = tex1;
        } else if (FragPos.y < texture2Level) {
            finalTexture = tex2;
        } else {
            finalTexture = tex3;
        }
        
        float normalizedHeight = 0.0;
        
        if (maxY > 0.0) {
            normalizedHeight = FragPos.y / maxY; // Normalize the y-coordinate of the fragment position for positive maxY
        } else if (minY < 0.0) {
            normalizedHeight = FragPos.y / minY; // Normalize the y-coordinate of the fragment position for negative maxY
        }

        vec4 gradientColor = vec4(normalizedHeight, normalizedHeight, normalizedHeight, 1.0);
        finalTexture *= gradientColor;

        FragColor = light * finalTexture;
    }
    else
    {
        float normalizedHeight = 0.0f;
        
        if (maxY > 0.0f)
        {
            normalizedHeight = FragPos.y / maxY;    // Normalize the y-coordinate of the fragment position for positive maxY
        }
        else if (minY < 0.0f)
        {
            normalizedHeight = (FragPos.y / minY);    // Normalize the y-coordinate of the fragment position for negative maxY. To have the white shown at the highest point and not
                                                      // the lowest, add "1 -" before "(FragPos.y / minY);" 
        }
        
        FragColor = vec4(normalizedHeight, normalizedHeight, normalizedHeight, 1.0); // Set the fragment color based on the normalized height
    }
}

vec4 ProcessDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // Get light direction
    vec3 lightDir = normalize(-light.direction);

    // Get diffuse intensity
    float diff = max(dot(normal, lightDir), 0.0);

    // Reflect light direction
    vec3 reflectDir = reflect(-lightDir, normal);

    // Get specular intensity
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    // Get result lights
    vec4 ambient = light.ambient;
    vec4 diffuse = light.diffuse * diff;
    vec4 specular = light.specular * spec;

    // Combine
    return ambient + diffuse + specular;
}

vec4 ProcessPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // Get light direction
    vec3 lightDir = normalize(light.position - fragPos);

    // Get diffuse intensity
    float diff = max(dot(normal, lightDir), 0.0);

    // Reflect light direction
    vec3 reflectDir = reflect(-lightDir, normal);

    // Get specular intensity
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    // Get the distance between the light and the pixel
    float distance = length(light.position - fragPos);

    // Compute light attenuation
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    // Get result lights
    vec4 ambient = light.ambient;
    vec4 diffuse = light.diffuse * diff;
    vec4 specular = light.specular * spec;

    // Apply attenuation
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // Combine
    return ambient + diffuse + specular;
}

vec4 ProcessSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // Get light direction
    vec3 lightDir = normalize(light.position - fragPos);

    // Get diffuse intensity
    float diff = max(dot(normal, lightDir), 0.0);

    // Reflect light direction
    vec3 reflectDir = reflect(-lightDir, normal);

    // Get specular intensity
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1);
    
    // Get the distance between the light and the pixel
    float dist = length(light.position - fragPos);
    
    // Compute light attenuation
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));    

    // Compute cutoff
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    // Get result lights
    vec4 ambient = light.ambient;
    vec4 diffuse = light.diffuse * diff;
    vec4 specular = light.specular * spec;

    // Apply attenuation
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    // Combine
    return ambient + diffuse + specular;
}