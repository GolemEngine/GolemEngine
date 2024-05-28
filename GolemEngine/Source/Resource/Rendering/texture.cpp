#include "Resource/Rendering/texture.h"

#define STB_IMAGE_IMPLEMENTATION 
#include "Image/stb_image.h"
#include "glad/glad.h"
#include "Debug/log.h"
#include "Wrappers/graphicWrapper.h"
#include "Utils/tools.h"


Texture::Texture()
{

}

Texture::Texture(unsigned int _width, unsigned int _height, unsigned int _format, unsigned int _internalFormat) :
    m_width(_width), m_height(_height)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, m_width, m_height, 0, _format, GL_UNSIGNED_BYTE, NULL);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}

void Texture::Load(const char* _filename)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(_filename, &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else
            format = GL_RGB;  // Default to RGB if nrChannels is unexpected

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        Log::Print("Succeeded to load texture %s", _filename);
    }
    else
    {
        Log::Print("Failed to load texture %s", _filename);
    }
    stbi_image_free(data);
}

void Texture::Use()
{
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::UnUse()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
