#version 460 core

struct VertexGpu {
    vec3 position;
    float padding1;
    vec3 normal;
    float padding2;
    vec2 textureCoords;
    vec2 padding3;
};

// readonly SSBO containing the data
layout(binding = 0, std430) readonly buffer ssbo1 {
    VertexGpu data[];
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float minHeight;
uniform float maxHeight;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out vec2 terrainPos;
out float minY; // Varying output for min height
out float maxY; // Varying output for max height


void main()
{
    VertexGpu vertex = data[gl_VertexID];
    
    terrainPos = vertex.position.xz;
    
    gl_Position = projection * view * model * vec4(vertex.position, 1.0);
    FragPos = vec3(model * vec4(vertex.position, 1.0));
    
    TexCoord = vertex.textureCoords;
    minY = minHeight; // Set the varying min height value
    maxY = maxHeight; // Set the varying max height value
}