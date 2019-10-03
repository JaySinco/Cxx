#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <vector>
#include "shader.h"
#include "texture.h"
#include "model.h"

namespace cxx {

class Scene;
class Repository {
public:
    friend Scene;
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
private:
    std::map<std::string, std::shared_ptr<Shader>> shaders;
    std::map<std::string, std::shared_ptr<Model>> models;
    std::map<std::string, std::shared_ptr<Texture>> textures;
};

class Object {
public:
    friend Scene;
    enum PROPERTY { SHADER, MODEL, TEXTURE };
    Object(const std::string &name);
    void load(PROPERTY property, const std::string &name);
    void resetPosition();
    void moveTo(float dx, float dy, float dz);
    void rotate(float degree, float axis_x, float axis_y, float axis_z);
private:
    glm::mat4 xform_model_translate = glm::mat4(1.0f);
    glm::mat4 xform_model_rotate= glm::mat4(1.0f);
    std::string shader_id, model_id;
    std::vector<std::string> textures_id;
    std::string id;
};

class Scene {
public:
    Scene(const std::string &name);
    std::shared_ptr<Object> newItem(const std::string &name);
    std::shared_ptr<Object> newItemCopy(
        const std::string &name,
        const std::shared_ptr<Object> &copy_from);
    std::shared_ptr<Object> getItemByName(const std::string &name);
    std::shared_ptr<Object> operator[](const std::string &name);
    void select(std::shared_ptr<Repository> repo);
    void setCamera(float fov_in_degree, float aspect, float near, float far);
    void moveCameraTo(float dx, float dy, float dz);
    void rotateCamera(float degree, float axis_x, float axis_y, float axis_z);
    void render();
private:
    void checkItemNotExist(const std::string &name) const;
    std::map<std::string, std::shared_ptr<Object>> item_map;
    std::shared_ptr<Repository> repository;
    glm::mat4 xform_view_translate = glm::mat4(1.0f);
    glm::mat4 xform_view_rotate = glm::mat4(1.0f);
    glm::mat4 xform_projection = glm::mat4(1.0f);
    std::string last_render_shader;
    std::string last_render_texture;
    std::string id;
};

}