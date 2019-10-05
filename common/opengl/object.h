#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>
#include <string>
#include "shader.h"
#include "texture.h"
#include "model.h"

namespace cxx {

namespace gl {

class Object {
public:
    enum PROPERTY { SHADER, MODEL, MATERIAL };
    Object(const std::string &name);
    Object(const std::string &name, const std::shared_ptr<Object> &copy_from);
    ~Object();
    void load(PROPERTY property, const std::string &name);
    void reset();
    void move(float dx, float dy, float dz);
    void moveTo(float x, float y, float z);
    void spin(float degree, float axis_x, float axis_y, float axis_z);
    void spinTo(float degree, float axis_x, float axis_y, float axis_z);
    glm::mat4 getModelMatrix();
    const std::string id;
    std::string shader_id, model_id, material_id;
private:
    glm::mat4 xform_model_translate = glm::mat4(1.0f);
    glm::mat4 xform_model_rotate= glm::mat4(1.0f);
};

} // namespace gl

} // namespace cxx