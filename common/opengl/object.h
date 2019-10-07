#pragma once
#include "base.h"

namespace cxx {

namespace gl {

class Object: public Base {
public:
    Object(
        const std::string &id,
        const std::string &model_id,
        const std::string &shader_id = "",
        const std::string &material_id = "",
        glm::mat4 xform_default = glm::mat4(1.0f));
    static glm::mat4 rotateX(float degree);
    static glm::mat4 rotateY(float degree);
    static glm::mat4 rotateZ(float degree);
    static glm::mat4 rotate(float degree, float axis_x, float axis_y, float axis_z);
    std::string model() const;
    std::string shader() const;
    std::string material() const;
    void reset();
    void move(float dx, float dy, float dz);
    void moveTo(float x, float y, float z);
    void spin(float degree, float axis_x, float axis_y, float axis_z);
    void spinTo(float degree, float axis_x, float axis_y, float axis_z);
    void scale(float x, float y, float z);
    void scaleTo(float dx, float dy, float dz);
    glm::mat4 getModelMatrix() const;
private:
    std::string model_;
    std::string shader_;
    std::string material_;
    const glm::mat4 xform_model_default;
    glm::mat4 xform_model_translate = glm::mat4(1.0f);
    glm::mat4 xform_model_rotate = glm::mat4(1.0f);
    glm::mat4 xform_model_scale = glm::mat4(1.0f);
};

} // namespace gl

} // namespace cxx