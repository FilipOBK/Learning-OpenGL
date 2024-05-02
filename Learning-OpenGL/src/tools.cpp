#include "tools.h"

#include <fstream>
#include <sstream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum err = glGetError())
    {
        std::clog << "[OpenGL Error] (" << err << ")\n\t"
            << function << "\n\t" << file << " : " << line
            << std::endl << std::flush;
        return false;
    }
    return true;
}

namespace tools
{
    std::string ReadFile(const std::string& path)
    {
        std::ifstream file_stream = std::ifstream(path);
        std::string res;
        if (file_stream)
        {
            std::stringstream ss;
            ss << file_stream.rdbuf();
            res = ss.str();
        }
        else
            std::clog << "Could not find file " << path << std::endl;
        return res;
    }
}