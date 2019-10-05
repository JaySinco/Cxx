#pragma once
#include <string>
#include <memory>

namespace cxx {

namespace gl {

class Texture {
public:
    Texture(const std::string &name, const std::string &path, bool flip_y_aixs = true);
    Texture(const Texture&) = delete;
    ~Texture();
    void use(unsigned index = 0) const;
    const std::string id;
private:
    unsigned texture;
};

} // namespace gl

} // namespace cxx