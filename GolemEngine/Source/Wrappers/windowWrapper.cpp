#include "Wrappers/windowWrapper.h"

#include <string>
#include <unordered_map>
#include <optional>

#include "Image/stb_image.h"


GLFWwindow* WindowWrapper::window = nullptr;
Vector2 WindowWrapper::screenSize = { 0.0f, 0.0f };

void WindowWrapper::InitWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
}

GLFWwindow* WindowWrapper::NewWindow(int _width, int _height, const char* _title, GLFWmonitor* _monitor, GLFWwindow* _share)
{
    return glfwCreateWindow(_width, _height, _title, _monitor, _share);
}

void WindowWrapper::Terminate()
{
    glfwTerminate();
}

int WindowWrapper::ShouldWindowClose(GLFWwindow* _window)
{
    return glfwWindowShouldClose(_window);
}

void WindowWrapper::ProcessEvents()
{
    glfwPollEvents();
}

void WindowWrapper::SwapBuffers(GLFWwindow* _window)
{
    glfwSwapBuffers(_window);
}

void WindowWrapper::MakeContext(GLFWwindow* _window)
{
    glfwMakeContextCurrent(_window);
}

std::unordered_map<std::string, std::optional<GLuint>> textureMap;

GLuint WindowWrapper::LoadUiTexture(const char* _filename)
{
    if (textureMap.find(_filename) != textureMap.end()) 
    {
        // Check if the texture is already loaded
        if (textureMap[_filename].has_value()) 
        {
            return textureMap[_filename].value();
        }
    }

    int width, height, channels;
    unsigned char* data = stbi_load(_filename, &width, &height, &channels, 0);
    if (!data)
    {
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    std::string filenameStr(_filename);
    std::string extension;
    size_t dotIndex = filenameStr.find_last_of('.');
    if (dotIndex != std::string::npos) {
        extension = filenameStr.substr(dotIndex);
    }

    GLenum internalFormat = GL_RGBA;
    if (extension == ".png") {
        internalFormat = GL_RGBA;
    }
    else if (extension == ".jpg") {
        internalFormat = GL_RGB;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);

    // Store texture ID in the map
    textureMap[_filename] = texture;

    return texture;
}

Vector2 WindowWrapper::GetScreenSize()
{
    return screenSize;
}

void WindowWrapper::SetScreenSize(Vector2 _screenSize)
{
    screenSize = { _screenSize.x, _screenSize.y };
}