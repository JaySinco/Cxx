#include "object.h"

namespace cxx
{

namespace gl
{

Object::Object(
    const std::string &id,
    const std::string &model_id,
    const std::string &shader_id,
    const std::string &material_id,
    glm::mat4 xform_default)
    : Base(Base::OBJECT, id), model_(model_id), shader_(shader_id),
      material_(material_id), xform_model_default(xform_default) {}

std::string Object::model() const
{
    return model_;
}

std::string Object::shader() const
{
    return shader_;
}

std::string Object::material() const
{
    return material_;
}

glm::mat4 Object::rotateX(float degree)
{
    return rotate(degree, 1.0f, 0.0f, 0.0f);
}

glm::mat4 Object::rotateY(float degree)
{
    return rotate(degree, 0.0f, 1.0f, 0.0f);
}

glm::mat4 Object::rotateZ(float degree)
{
    return rotate(degree, 0.0f, 0.0f, 1.0f);
}

glm::mat4 Object::rotate(float degree, float axis_x, float axis_y, float axis_z)
{
    return glm::rotate(glm::mat4(1.0f),
                       glm::radians(degree),
                       glm::vec3(axis_x, axis_y, axis_z));
}

void Object::reset()
{
    xform_model_translate = glm::mat4(1.0f);
    xform_model_rotate = glm::mat4(1.0f);
}

void Object::move(float dx, float dy, float dz)
{
    xform_model_translate = glm::translate(
                                glm::mat4(1.0f),
                                glm::vec3(dx, dy, dz)) *
                            xform_model_translate;
}

void Object::moveTo(float x, float y, float z)
{
    xform_model_translate = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Object::spin(float degree, float axis_x, float axis_y, float axis_z)
{
    xform_model_rotate = rotate(degree, axis_x, axis_y, axis_z) * xform_model_rotate;
}

void Object::spinTo(float degree, float axis_x, float axis_y, float axis_z)
{
    xform_model_rotate = rotate(degree, axis_x, axis_y, axis_z);
}

void Object::scale(float x, float y, float z)
{
    xform_model_scale = glm::scale(xform_model_scale, glm::vec3(x, y, z));
}

void Object::scaleTo(float dx, float dy, float dz)
{
    xform_model_scale = glm::scale(glm::mat4(1.0f), glm::vec3(dx, dy, dz));
}

glm::mat4 Object::getModelMatrix() const
{
    return xform_model_translate * xform_model_rotate * xform_model_scale * xform_model_default;
}

} // namespace gl

} // namespace cxx