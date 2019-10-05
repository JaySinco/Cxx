#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <memory>

namespace cxx {

namespace gl {

class Shader {
public:
    Shader(
        const std::string &name,
        const std::string &vsPath,
        const std::string &fsPath);
    Shader(const Shader&) = delete;
    ~Shader();
    void use() const;
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setMat4(const std::string &name, const glm::mat4 &value) const;
    const std::string id;
private:
    void checkError(unsigned shader, const std::string &type) const;
    unsigned program;
};

} // namespace gl

} // namespace cxx