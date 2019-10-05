#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <glad/glad.h>
#include "object.h"

namespace cxx {

namespace gl {
    
Object::Object(const std::string &name): id(name) {}

Object::Object(const std::string &name, const std::shared_ptr<Object> &cp)
    : id(name), shader_id(cp->shader_id), model_id(cp->shader_id), material_id(cp->material_id) {}

Object::~Object() {
    LOG(INFO) << "delete object, id=\"" << id << "\"";
}

void Object::load(PROPERTY property, const std::string &name) {
    switch(property) {
        case MODEL: model_id = name; break;
        case SHADER: shader_id = name; break;
        case MATERIAL: material_id = name; break;
    }
}

void Object::reset() {
    xform_model_translate = glm::mat4(1.0f);
    xform_model_rotate= glm::mat4(1.0f);
 }

void Object::move(float dx, float dy, float dz) {
    xform_model_translate = glm::translate(xform_model_translate, glm::vec3(dx, dy, dz));
}

void Object::moveTo(float x, float y, float z) {
    xform_model_translate = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Object::spin(float degree, float axis_x, float axis_y, float axis_z) {
    xform_model_rotate = glm::rotate(
        xform_model_rotate, 
        glm::radians(degree),
        glm::vec3(axis_x, axis_y, axis_z));
}

void Object::spinTo(float degree, float axis_x, float axis_y, float axis_z) {
    xform_model_rotate = glm::rotate(
        glm::mat4(1.0f), 
        glm::radians(degree),
        glm::vec3(axis_x, axis_y, axis_z));
}

glm::mat4 Object::getModelMatrix() {
    return xform_model_translate * xform_model_rotate;
}

} // namespace gl

} // namespace cxx