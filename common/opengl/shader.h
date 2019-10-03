#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <memory>

namespace cxx {

class Shader {
public:
    Shader(
        const std::string &name,
        const std::string &vsPath,
        const std::string &fsPath);
    Shader(const Shader&) = delete;
    ~Shader();
    void use() const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 &value) const;
private:
    void checkError(unsigned shader, const std::string &type) const;
    unsigned program;
    std::string id;
};

}