#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "shader.h"

namespace cxx {

Shader::Shader(
        const std::string &name,
        const std::string &vsPath, 
        const std::string &fsPath): id(name) {
    std::ifstream vsFile(vsPath), fsFile(fsPath);
    if (!vsFile) {
        LOG(ERROR) << "failed to load vertex shader, vsPath=\"" << vsPath << "\"";
        exit(-1);
    }
    if (!fsFile) {
        LOG(ERROR) << "failed to load fragment shader, fsPath=\"" << fsPath << "\"";
        exit(-1);
    }
    // read source code
    std::ostringstream vsStream, fsStream;
    vsStream << vsFile.rdbuf();
    fsStream << fsFile.rdbuf();
    std::string vsSource(vsStream.str()), fsSource(fsStream.str());
    const char *vsCode(vsSource.c_str()), *fsCode(fsSource.c_str());
    // vertex shader
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vsCode, NULL);
    glCompileShader(vs);
    checkError(vs, "VERTEX");
    // fragment shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fsCode, NULL);
    glCompileShader(fs);
    checkError(fs, "FRAGMENT");
    // shader program
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    checkError(program, "PROGRAM");
    // delete shaders
    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader() {
    LOG(INFO) << "delete program, id=\"" << id << "\"";
    glDeleteProgram(program);
}

void Shader::use() const { 
    glUseProgram(program);
}

void Shader::setInt(const std::string &name, int value) const {
    use();
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    use();
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &value) const {
    use();
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::checkError(GLuint shader, const std::string &type) const {
    const int bufSize = 1024;
    int success;
    char errorMsg[bufSize];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, bufSize, NULL, errorMsg);
            LOG(ERROR) << "failed to compile " << type << ", errorMsg=\"" << errorMsg << "\"";
            exit(-1);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, bufSize, NULL, errorMsg);
            LOG(ERROR) << "failed to link " << type << ", errorMsg=\"" << errorMsg << "\"";
            exit(-1);
        }
    }
}

}
