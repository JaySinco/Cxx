#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

namespace cxx {

class Shader {
public:
    Shader(const std::string &name, const std::string &vsPath, const std::string &fsPath);
    Shader(const Shader&) = delete;
    ~Shader() {
        LOG(INFO) << "delete program, id=\"" << id << "\"";
        glDeleteProgram(program);
     }
    void use() const { 
        glUseProgram(program);
    }
    void setInt(const std::string &name, int value) const {
        use();
        glUniform1i(glGetUniformLocation(program, name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const {
        use();
        glUniform1f(glGetUniformLocation(program, name.c_str()), value);
    }
private:
    void checkError(unsigned shader, const std::string &type);
    unsigned program;
    std::string id;
};

}