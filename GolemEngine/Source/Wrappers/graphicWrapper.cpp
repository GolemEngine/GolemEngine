#include "Wrappers/graphicWrapper.h"
#include "Core/gameobject.h"

#include <iostream>
#include <GLFW/glfw3.h>

#include "Debug/log.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix4.h"
#include "Wrappers/windowWrapper.h"

unsigned int GraphicWrapper::m_vao;
unsigned int GraphicWrapper::m_vbo;
unsigned int GraphicWrapper::m_rbo;
unsigned int GraphicWrapper::m_fbo;

unsigned int GraphicWrapper::m_playSceneVao;
unsigned int GraphicWrapper::m_playSceneVbo;
unsigned int GraphicWrapper::m_playSceneRbo;
unsigned int GraphicWrapper::m_playSceneFbo;
unsigned int GraphicWrapper::m_playSceneId;

int GraphicWrapper::Init()
{
    textures.resize(3);
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void GraphicWrapper::CreateFramebuffer(int _width, int _height)
{
    unsigned int attachments[3];

    // Create framebuffer
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    // Create renderBuffer
    CreateRenderBuffer(_width, _height, m_rbo);

    // Create textureBuffer
    for (int i = 0; i < textures.size(); i++)
    {
        if (i == 0)
        {
            textures[0] = std::make_unique<Texture>(_width, _height, GL_RGBA, GL_RGBA);
        }
        else if (i == 1)
        {
            textures[i] = std::make_unique<Texture>(_width, _height, GL_RED_INTEGER, GL_R32I);
        }
        else
        {
            textures[i] = std::make_unique<Texture>(_width, _height, GL_RG, GL_RG32F);
        }
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textures[i]->id, 0);
        attachments[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    AttachTexture(GL_RGBA, _width, _height, GL_COLOR_ATTACHMENT0, textures[0]->id, m_fbo);
    glDrawBuffers(3, attachments);

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    
    // Check framebuffer completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "Framebuffer is not complete." << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);   // Unbind framebuffer
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);   // Unbind framebuffer
    glBindTexture(GL_TEXTURE_2D, 0);    // Unbind texture
    glBindRenderbuffer(GL_RENDERBUFFER, 0); // Unbind renderbuffer

    #pragma region playscene textureid
    // Create PlayScene framebuffer
    glGenFramebuffers(1, &m_playSceneFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_playSceneFbo);

    // Create PlayScene texturebuffer
    glGenTextures(1, &m_playSceneId);
    glBindTexture(GL_TEXTURE_2D, m_playSceneId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_playSceneId, 0);
    AttachTexture(GL_RGBA, _width, _height, 0, m_playSceneId, m_playSceneFbo);

    CreateRenderBuffer(_width, _height, m_playSceneRbo);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);   // Unbind framebuffer
    glBindTexture(GL_TEXTURE_2D, 0);    // Unbind texture
    glBindRenderbuffer(GL_RENDERBUFFER, 0); // Unbind renderbuffer
    #pragma endregion playscene textureid
}

void GraphicWrapper::AttachTexture(unsigned int _format, int _width, int _height, unsigned int _attachment, unsigned int _id, unsigned int _fbo)
{
    glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    glBindTexture(GL_TEXTURE_2D, _id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED_INTEGER, _width, _height, 0, _format, GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _attachment, GL_TEXTURE_2D, _id, 0);
}

void GraphicWrapper::CreateRenderBuffer(int _width, int _height, unsigned int _rbo)
{
    // Create renderbuffer
    glGenRenderbuffers(1, &_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);
}

int GraphicWrapper::ReadPixelFromIndexBuffer(int _x, int _y)
{
    glReadBuffer(GL_COLOR_ATTACHMENT0 + 1);
    int pixelData;
    glReadPixels(_x, _y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
    return pixelData;
}

Vector2 GraphicWrapper::ReadPixelFromUVBuffer(int _x, int _y)
{
    glFlush();
    glFinish();
    BindFramebuffer();
    glReadBuffer(GL_COLOR_ATTACHMENT0 + 2);
    Vector2 pixelData;
    glReadPixels(_x, _y, 1, 1, GL_RG, GL_FLOAT, &pixelData);
    return pixelData;
}

void GraphicWrapper::ClearBuffer()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GraphicWrapper::BindFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void GraphicWrapper::BindPlaySceneFrambuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_playSceneFbo);
}

void GraphicWrapper::UnbindFramebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GraphicWrapper::EnableDepth()
{
    glEnable(GL_DEPTH_TEST);
}

unsigned int GraphicWrapper::GetTextureId()
{
    return textures[0]->id;
}

unsigned int GraphicWrapper::GetPlaySceneId()
{
    return m_playSceneId;
}

void GraphicWrapper::SetBackgroundColor(Vector4 _color)
{
    glClearColor(_color.x, _color.y, _color.z, _color.w);
}

void GraphicWrapper::SetViewport(GLint _xMin, GLint _yMin, GLsizei _xMax, GLsizei _yMax)
{
    glViewport(_xMin, _yMin, _xMax, _yMax);
}


#pragma region Shader functions

void GraphicWrapper::GetShaderIv(GLuint _shader, GLenum _pName, GLint* _params)
{
    glGetShaderiv(_shader, _pName, _params);
}

void GraphicWrapper::GetShaderLog(GLuint _shader, GLsizei _bufSize, GLsizei* _length, GLchar* _infoLog)
{
    glGetShaderInfoLog(_shader, _bufSize, _length, _infoLog);
}

GLuint GraphicWrapper::CreateShader(GLenum _shaderType)
{
    return glCreateShader(_shaderType);
}

void GraphicWrapper::SetShaderSourceCode(GLuint _shader, GLsizei _count, const GLchar** _string, const GLint* _length)
{
    glShaderSource(_shader, _count, _string, _length);
}

void GraphicWrapper::CompileShader(GLuint _shader)
{
    glCompileShader(_shader);
}

GLuint GraphicWrapper::CreateShaderProgram()
{
    return glCreateProgram();
}

void GraphicWrapper::AttachShaderToProgram(GLuint _program, GLuint _shader)
{
    glAttachShader(_program, _shader);
}

void GraphicWrapper::LinkProgram(GLuint _program)
{
    glLinkProgram(_program);
}

void GraphicWrapper::DeleteShaderObject(GLuint _shader)
{
    glDeleteShader(_shader);
}

void GraphicWrapper::UseShader(GLuint _program)
{
    glUseProgram(_program);
}

unsigned int GraphicWrapper::GetFbo()
{
    return m_fbo;
}

void GraphicWrapper::SetShaderBool(GLuint _program, const std::string& _name, bool _value)
{
    glUniform1i(glGetUniformLocation(_program, _name.c_str()), (int)_value);
}

void GraphicWrapper::SetShaderInt(GLuint _program, const std::string& _name, int _value)
{
    glUniform1i(glGetUniformLocation(_program, _name.c_str()), _value);
}

void GraphicWrapper::SetShaderFloat(GLuint _program, const std::string& _name, float _value)
{
    glUniform1f(glGetUniformLocation(_program, _name.c_str()), _value);
}

void GraphicWrapper::SetShaderVec2(GLuint _program, const std::string& _name, const Vector2& _value)
{
    glUniform2fv(glGetUniformLocation(_program, _name.c_str()), 1, &_value.x);
}

void GraphicWrapper::SetShaderVec2(GLuint _program, const std::string& _name, float _x, float _y)
{
    glUniform2f(glGetUniformLocation(_program, _name.c_str()), _x, _y);
}

void GraphicWrapper::SetShaderVec3(GLuint _program, const std::string& _name, const Vector3& _value)
{
    glUniform3fv(glGetUniformLocation(_program, _name.c_str()), 1, &_value.x);
}

void GraphicWrapper::SetShaderVec3(GLuint _program, const std::string& _name, float _x, float _y, float _z)
{
    glUniform3f(glGetUniformLocation(_program, _name.c_str()), _x, _y, _z);
}

void GraphicWrapper::SetShaderVec4(GLuint _program, const std::string& _name, const Vector4& _value)
{
    glUniform4fv(glGetUniformLocation(_program, _name.c_str()), 1, &_value.x);
}

void GraphicWrapper::SetShaderVec4(GLuint _program, const std::string& _name, float _x, float _y, float _z, float _w)
{
    glUniform4f(glGetUniformLocation(_program, _name.c_str()), _x, _y, _z, _w);
}

void GraphicWrapper::SetShaderMat4(GLuint _program, const std::string& _name, const Matrix4& _mat)
{
    glUniformMatrix4fv(glGetUniformLocation(_program, _name.c_str()), 1, GL_TRUE, &_mat.data[0][0]);
}

#pragma endregion Shader functions