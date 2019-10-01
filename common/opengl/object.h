#pragma once
#include "shader.h"
#include "texture.h"
#include "model.h"

namespace cxx {

class Object {
public:
    Object(const std::string &name);
    void load(std::shared_ptr<Shader> shader_ptr);
    void load(std::shared_ptr<Model> model_ptr);
    void load(std::shared_ptr<Texture> texture_ptr);
    void render() const;
private:
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Model> model;
    std::shared_ptr<Texture> texture;
    std::string id;
};

}