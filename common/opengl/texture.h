#pragma once
#include <string>
#include <memory>

namespace cxx {

class Texture {
public:
    Texture(const std::string &name, const std::string &path, bool flip_y_aixs = true);
    Texture(const Texture&) = delete;
    ~Texture();
    void use(unsigned index = 0) const;
private:
    unsigned texture;
    std::string id;
};

}