#version 460 core
out vec4 FragColor;
out int FragColor2;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{   
    FragColor = texture(skybox, TexCoords);
    FragColor2 = -1;
}