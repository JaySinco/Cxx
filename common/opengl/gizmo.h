#pragma once
#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace cxx {

namespace gl {

class Material {
public:
    Material(
        const std::string &name,
        const std::string &diff_texture_id,
        float specular,
        float shine):
            diffuse_texture_id(diff_texture_id), 
            specularVec(specular),
            shininess(shine),
            id(name) {};
    ~Material() {
        LOG(INFO) << "delete material, id=\"" << id << "\"";
    };
    std::string diffuse_texture_id;
    glm::vec3 specularVec;
    float shininess;
    const std::string id;
};

class Storage {
public:
    Storage(const std::string &name): id(name) {};
    ~Storage() {
        LOG(INFO) << "delete storage, id=\"" << id << "\"";
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
    template<typename... Args>
    void addMaterial(const std::string &name, Args&&... args) {
        materials[name] = std::make_shared<Material>(name, args...);    
    }
    std::map<std::string, std::shared_ptr<Shader>> shaders;
    std::map<std::string, std::shared_ptr<Model>> models;
    std::map<std::string, std::shared_ptr<Texture>> textures;
    std::map<std::string, std::shared_ptr<Material>> materials;
    const std::string id;
};

class Light {
public:
    Light(
        const std::string &name,
        glm::vec3 pos, glm::vec3 color,
        float ambient = 0.5f, float diffuse = 0.8f, float specular = 1.0f): 
            posVec(pos),
            ambientVec(ambient*color), 
            diffuseVec(diffuse*color), 
            specularVec(specular*color),
            id(name) {};
    Light(const Light&) = delete;
    ~Light()  {
        LOG(INFO) << "delete light, id=\"" << id << "\"";
    };
    glm::vec3 posVec;
    glm::vec3 ambientVec;
    glm::vec3 diffuseVec;
    glm::vec3 specularVec;
    const std::string id;
};

} // namespace gl

} // namespace cxx