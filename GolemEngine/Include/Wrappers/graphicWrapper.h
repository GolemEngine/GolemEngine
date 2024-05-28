#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <memory>
#include <string>

#include "dll.h"
#include "Resource/Rendering/texture.h"

#define OPENGL_MAJOR_VERSION GLFW_CONTEXT_VERSION_MAJOR
#define OPENGL_MINOR_VERSION GLFW_CONTEXT_VERSION_MINOR

#pragma region Shader macros
#define SHADER_COMPILE_STATUS GL_COMPILE_STATUS
#define SHADER_LINK_STATUS GL_LINK_STATUS
#define SHADER_TYPE_VERTEX GL_VERTEX_SHADER
#define SHADER_TYPE_FRAGMENT GL_FRAGMENT_SHADER
#define SHADER_TYPE_COMPUTE GL_COMPUTE_SHADER
#pragma endregion Shader macros

class Vector2;
class Vector3;
class Vector4;
class Matrix4;

class GraphicWrapper
{
private:
	static unsigned int m_vao;
	static unsigned int m_vbo;
	static unsigned int m_rbo;
	static unsigned int m_fbo;
	static int m_width;
	static int m_height;

	static unsigned int m_playSceneVao;
	static unsigned int m_playSceneVbo;
	static unsigned int m_playSceneRbo;
	static unsigned int m_playSceneFbo;
	static unsigned int m_playSceneId;

public:
	GOLEM_ENGINE_API static inline std::vector<std::unique_ptr<Texture>> textures;

private:
	// Private constructors and destructor to make static class
	GraphicWrapper() = delete;
	GraphicWrapper(GraphicWrapper& _other) = delete;
	GraphicWrapper(GraphicWrapper&& _other) = delete;
	GraphicWrapper& operator=(GraphicWrapper& _other) = delete;
	GraphicWrapper& operator=(GraphicWrapper&& _other) = delete;
	void operator=(const GraphicWrapper&) = delete;
	~GraphicWrapper() = delete;

public:
	// Initialize library
	GOLEM_ENGINE_API static int Init();
	// Used to CREATE the scene in a framebuffer to be used as a texture. We do this in order to pass the texture to an ImGUI window so it can display it. 
	GOLEM_ENGINE_API static void CreateFramebuffer(int _width, int _height);

	GOLEM_ENGINE_API static void AttachTexture(unsigned int _format, int _width, int _heigh, unsigned int _attachment, unsigned int _id, unsigned int _fbo);

	GOLEM_ENGINE_API static void CreateRenderBuffer(int _width, int _height, unsigned int _rbo);
	// Used to clear a buffer for the next draw
	GOLEM_ENGINE_API static void ClearBuffer();
	// Used to BIND the scene in a framebuffer to be used as a texture. We do this in order to pass the texture to an ImGUI window so it can display it. 
	// The result is that we get a movable viewport in an ImGUI window.
	GOLEM_ENGINE_API static void BindFramebuffer();

	GOLEM_ENGINE_API static void BindPlaySceneFrambuffer();
	// Used to UNBIND the scene from a framebuffer to free the framebuffer. 
	GOLEM_ENGINE_API static void UnbindFramebuffer();
	// Enable a feature of OpenGl
	GOLEM_ENGINE_API static void EnableDepth();

	GOLEM_ENGINE_API static int ReadPixelFromIndexBuffer(int _x, int _y);
	GOLEM_ENGINE_API static Vector2 ReadPixelFromUVBuffer(int _x, int _y);

	// This function is used to retrieve the texture of a framebuffer to use in the viewport to show the scene as a texture.
	GOLEM_ENGINE_API static unsigned int GetTextureId();

	// This function is used to renderer a play scene with player view camera
	GOLEM_ENGINE_API static unsigned int GetPlaySceneId();

	// Used to set the background color of a buffer. Color is in the form of rgba vector. 
	GOLEM_ENGINE_API static void SetBackgroundColor(Vector4 _color);
	// Used to set the viewport that will be used
	GOLEM_ENGINE_API static void SetViewport(GLint _xMin, GLint _yMin, GLsizei _xMax, GLsizei _yMax);

	GOLEM_ENGINE_API static unsigned int GetFbo();

#pragma region Shader functions
	// These functions should only be used through the Shader class and not through this GraphicWrapper class

	// Get parameters of the shader
	GOLEM_ENGINE_API static void GetShaderIv(GLuint _shader, GLenum _pName, GLint* _params);
	// Get info log of shader
	GOLEM_ENGINE_API static void GetShaderLog(GLuint _shader, GLsizei _bufSize, GLsizei* _length, GLchar* _infoLog);
	// Create shader
	GOLEM_ENGINE_API static GLuint CreateShader(GLenum _shaderType);
	// Set the source code of the shader
	GOLEM_ENGINE_API static void SetShaderSourceCode(GLuint _shader, GLsizei _count, const GLchar** _string, const GLint* _length);
	// Compile shader
	GOLEM_ENGINE_API static void CompileShader(GLuint _shader);
	// Create and return the program to set, use and manage shaders
	GOLEM_ENGINE_API static GLuint CreateShaderProgram();
	// Attach shader to the program
	GOLEM_ENGINE_API static void AttachShaderToProgram(GLuint _program, GLuint _shader);
	// Link the shader program
	GOLEM_ENGINE_API static void LinkProgram(GLuint _program);
	// Delete the shader object
	GOLEM_ENGINE_API static void DeleteShaderObject(GLuint _shader);
	// Use shader
	GOLEM_ENGINE_API static void UseShader(GLuint _program);

	GOLEM_ENGINE_API static void SetShaderBool(GLuint _program, const std::string& _name, bool _value);
	GOLEM_ENGINE_API static void SetShaderInt(GLuint _program, const std::string& _name, int _value);
	GOLEM_ENGINE_API static void SetShaderFloat(GLuint _program, const std::string& _name, float _value);
	GOLEM_ENGINE_API static void SetShaderVec2(GLuint _program, const std::string& _name, const Vector2& _value);
	GOLEM_ENGINE_API static void SetShaderVec2(GLuint _program, const std::string& _name, float _x, float _y);
	GOLEM_ENGINE_API static void SetShaderVec3(GLuint _program, const std::string& _name, const Vector3& _value);
	GOLEM_ENGINE_API static void SetShaderVec3(GLuint _program, const std::string& _name, float _x, float _y, float _z);
	GOLEM_ENGINE_API static void SetShaderVec4(GLuint _program, const std::string& _name, const Vector4& _value);
	GOLEM_ENGINE_API static void SetShaderVec4(GLuint _program, const std::string& _name, float _x, float _y, float _z, float _w);
	GOLEM_ENGINE_API static void SetShaderMat4(GLuint _program, const std::string& _name, const Matrix4& _mat);

#pragma endregion Shader functions
};