#pragma once

#include <GL/glew.h>
#include <map>
#include <string>

struct ShaderProgramSource
{
	std::string vertex_source;
	std::string fragment_source;
};

class Shader
{
private:
	std::string m_FilePath_Vertex;
	std::string m_FilePath_Fragment;
	unsigned int m_RendererID;
	mutable std::map<std::string, GLint> m_Uniform_Location_Cache;
public:
	Shader(const std::string& filepath_Vertex, const std::string& filepath_Fragment);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int x) const;
	void SetUniform1f(const std::string& name, float x) const;
	void SetUniform2f(const std::string& name, float x, float y) const;
private:
	unsigned int CompileShader(GLuint type, const std::string& source);
	unsigned int CreateShader(const std::string& vertex_shader, const std::string& fragment_shader);
	GLint GetUniformLocation(const std::string& uniform) const;
};