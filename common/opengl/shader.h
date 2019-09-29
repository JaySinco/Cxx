#ifndef COMMON_OPENGL_SHADER_H
#define COMMON_OPENGL_SHADER_H

#include <string>

class Shader
{
public:
    unsigned int ID;

    // constructor generates the shader on the fly
    Shader(const char* vertexPath, const char* fragmentPath);
    
    // activate the shader
    void use();

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif