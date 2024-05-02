#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

namespace tools
{
    uint32_t CompileShader(GLenum type, const std::string& source);

    uint32_t CreateShader(const std::string& vertex_shader, const std::string& fragment_shader);

    std::string ReadFile(const std::string& path);
}