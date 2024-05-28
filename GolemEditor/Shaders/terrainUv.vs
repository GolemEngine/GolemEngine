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

out vec2 TexCoord;

void main()
{
    VertexGpu vertex = data[gl_VertexID];
    
    gl_Position = projection * view * model * vec4(vertex.position, 1.0);
    
    TexCoord = vertex.position.xz;
}