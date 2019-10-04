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

class Object;
class Repository {
public:
    Repository(const std::string &name): id(name) {};
    ~Repository() {
        LOG(INFO) << "delete repository, id=\"" << id << "\"";
    }
    template<typename... Args> 
    void addModel(const std::string &name, Args&&... args) {
        models[name] = std::make_shared<Model>(name, args...);
    }
    template<typename... Args>
    void addShader(const std::string &name, Args&&... args) {
        shaders[name] = std::make_shared<Shader>(name, args...);
    }
    template<typename... Args>
    void addTexture(const std::string &name, Args&&... args) {
        textures[name] = std::make_shared<Texture>(name, args...);    
    }
    std::map<std::string, std::shared_ptr<Shader>> shaders;
    std::map<std::string, std::shared_ptr<Model>> models;
    std::map<std::string, std::shared_ptr<Texture>> textures;
    const std::string id;
};

class Object {
public:
    enum PROPERTY { SHADER, MODEL, TEXTURE };
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
    std::string shader_id, model_id;
    std::vector<std::string> textures_id;
private:
    glm::mat4 xform_model_translate = glm::mat4(1.0f);
    glm::mat4 xform_model_rotate= glm::mat4(1.0f);
};

}