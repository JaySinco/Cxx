#pragma once
#include <string>
#include <memory>

namespace cxx {

class Texture {
public:
    static std::shared_ptr<Texture> fromFile(const std::string &name, 
        const std::string &path, bool flip = false);
    Texture(const std::string &name, const std::string &path, bool flip);
    Texture(const Texture&) = delete;
    ~Texture();
    void use(unsigned index = 0) const;
private:
    unsigned texture;
    std::string id;
};

}