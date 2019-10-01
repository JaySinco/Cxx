#pragma once
#include <string>
#include <memory>

namespace cxx {

class Shader {
public:
    static std::shared_ptr<Shader> fromFile(const std::string &name, 
        const std::string &vsPath, const std::string &fsPath);
    Shader(const std::string &name, const std::string &vsPath, const std::string &fsPath);
    Shader(const Shader&) = delete;
    ~Shader();
    void use() const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
private:
    void checkError(unsigned shader, const std::string &type) const;
    unsigned program;
    std::string id;
};

}