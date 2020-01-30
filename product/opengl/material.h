#pragma once
#include "base.h"

namespace cxx {

namespace gl {

class Material: public Base {
public:
    Material(
        const std::string &id,
        const std::string &diffuse_texture_id,
        float specular,
        float shineness):
            Base(Base::MATERIAL, id),
            diff_texture_(diffuse_texture_id), 
            specular_(specular),
            shininess_(shineness) {};
    std::string diff_texture() const { return diff_texture_; }
    glm::vec3 specular() const { return specular_; }
    float shininess() const { return shininess_; }
private:
    std::string diff_texture_;
    glm::vec3 specular_;
    float shininess_;
};

} // namespace gl

} // namespace cxx