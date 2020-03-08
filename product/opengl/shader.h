#pragma once
#include "base.h"

namespace cxx {

namespace gl {

    class Shader : public Base {

    public:
        Shader(
            const std::string& id,
            const std::string& vsPath,
            const std::string& fsPath);
        ~Shader();
        void use() const;
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setMat4(const std::string& name, const glm::mat4& value) const;

    private:
        void checkError(unsigned shader, const std::string& type) const;
        unsigned program;
    };

} // namespace gl

} // namespace cxx