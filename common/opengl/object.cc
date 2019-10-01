#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <numeric>
#include "object.h"

namespace cxx {

Object::Object(const std::string &name): id(name) {}

void Object::load(std::shared_ptr<Shader> shader_ptr) {
    shader = shader_ptr;
}

void Object::load(std::shared_ptr<Model> model_ptr) {
    model = model_ptr;
}

void Object::load(std::shared_ptr<Texture> texture_ptr) {
    texture = texture_ptr;
}

void Object::render() const {
    if (texture) texture->use();
    if (shader) shader->use();
    if (!model) {
        LOG(ERROR) << "haven't load model yet, object id=\"" << id << "\"";
        exit(-1);
    }
    model->draw();
}

}