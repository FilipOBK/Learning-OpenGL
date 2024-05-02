#include "Shader.h"
#include "tools.h"

Shader::Shader(const std::string& filepath_Vertex, const std::string& filepath_Fragment)
	: m_FilePath_Vertex(filepath_Vertex), m_FilePath_Fragment(filepath_Fragment)
{
	ShaderProgramSource source = { tools::ReadFile(filepath_Vertex), tools::ReadFile(filepath_Fragment) };
    m_RendererID = CreateShader(source.vertex_source, source.fragment_source);

}

Shader::~Shader()
{
	GLCALL(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
	GLCALL(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCALL(glUseProgram(0));
}

void Shader::SetUniform1f(const std::string& name, float x) const
{
    GLuint location = GetUniformLocation(name);
	GLCALL(glUniform1f(location, x));
}

void Shader::SetUniform2f(const std::string& name, float x, float y) const
{
    GLuint location = GetUniformLocation(name);
	GLCALL(glUniform2f(location, x, y));
}



unsigned int Shader::CompileShader(GLuint type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // OpenGL wants a raw string
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) // shader did not compile successfully
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        std::unique_ptr<char> message = std::make_unique<char>(length);
        glGetShaderInfoLog(id, length, &length, message.get());
        std::clog << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader!\n\t" << message.get() << std::endl << std::flush;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertex_shader, const std::string& fragment_shader)
{
    uint32_t program = glCreateProgram();
    uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
    uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

    // Link these shaders into 1 program
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    // Logic is now stored inside program, we can now get rid of shaders
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

GLint Shader::GetUniformLocation(const std::string & uniform) const
{
	if (m_Uniform_Location_Cache.find(uniform) != m_Uniform_Location_Cache.end())
		return m_Uniform_Location_Cache[uniform];

	m_Uniform_Location_Cache[uniform] = glGetUniformLocation(m_RendererID, uniform.c_str());
	return m_Uniform_Location_Cache[uniform];
}
