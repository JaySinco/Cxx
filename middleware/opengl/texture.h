#pragma once
#include "base.h"

namespace cxx {

namespace gl {

class Texture: public Base {
public:
    Texture(const std::string &id, const std::string &path, bool flip_y_aixs = true);
    ~Texture();
    void use(unsigned index = 0) const;
private:
    unsigned texture;
};

} // namespace gl

} // namespace cxx