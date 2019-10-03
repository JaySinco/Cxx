#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <glad/glad.h>
#include <numeric>
#include "engine.h"

namespace cxx {

Object::Object(const std::string &name): id(name) {}

void Object::load(PROPERTY property, const std::string &name) {
    switch(property) {
        case MODEL: model_id = name; break;
        case SHADER: shader_id = name; break;
        case TEXTURE: textures_id.push_back(name); break;
    }
}

 void Object::reset() {
    xform_model_translate = glm::mat4(1.0f);
    xform_model_rotate= glm::mat4(1.0f);
 }

void Object::moveTo(float dx, float dy, float dz) {
    xform_model_translate = glm::translate(glm::mat4(1.0f), glm::vec3(dx, dy, dz));
}

void Object::rotate(float degree, float axis_x, float axis_y, float axis_z) {
    xform_model_rotate = glm::rotate(
        xform_model_rotate, 
        glm::radians(degree),
        glm::vec3(axis_x, axis_y, axis_z));
}

Scene::Scene(const std::string &name): id(name) {}

std::shared_ptr<Object> Scene::newItem(const std::string &name) {
    checkItemNotExist(name);
    auto item = std::make_shared<Object>(name);
    item_map[name] = item;
    return item;
}

std::shared_ptr<Object> Scene::newItemCopy(
        const std::string &name,
        const std::shared_ptr<Object> &copy_from) {
    checkItemNotExist(name);
    auto item = std::make_shared<Object>(*copy_from);
    item->id = name;
    item_map[name] = item;
    return item;
}

std::shared_ptr<Object> Scene::getItemByName(const std::string &name) {
    return item_map.at(name);
}

std::shared_ptr<Object> Scene::operator[](const std::string &name) {
    return getItemByName(name);
}

void Scene::checkItemNotExist(const std::string &name) const {
    if (item_map.find(name) != item_map.end()) {
        LOG(ERROR) << "can't add new item(" << "id=\"" << name 
            << "\"), already exists in scene(" << "id=\"" << id << "\""; 
        exit(-1);
    }
}

void Scene::select(std::shared_ptr<Repository> repo) {
    repository = repo;
}

void Scene::setCamera(float fov_in_degree, float aspect, float near, float far) {
    xform_projection = glm::perspective(
        glm::radians(fov_in_degree),
        aspect, near, far);
}

void Scene::moveCameraTo(float dx, float dy, float dz) {
    xform_view_translate = glm::translate(glm::mat4(1.0f), glm::vec3(-dx, -dy, -dz)); 
}

void Scene::rotateCamera(float degree, float axis_x, float axis_y, float axis_z) {
    xform_view_rotate = glm::rotate(
        xform_view_rotate,
        glm::radians(-1 * degree),
        glm::vec3(axis_x, axis_y, axis_z));
}

void Scene::resetCamera() {
    xform_view_translate = glm::mat4(1.0f);
    xform_view_rotate = glm::mat4(1.0f);
}

void Scene::render() {
    for (const auto &item_pair: item_map) {
        auto item = item_pair.second;
        
        if (item->shader_id != "" && 
                (last_render_shader == "" || last_render_shader != item->shader_id)) {
            repository->shaders.at(item->shader_id)->use();
            last_render_shader = item->shader_id;
        }
        auto current_shader = repository->shaders.at(last_render_shader);
        current_shader->setMat4("uf_xform_model",
            item->xform_model_translate * item->xform_model_rotate);
        current_shader->setMat4("uf_xform_view",
            xform_view_translate * xform_view_rotate);
        current_shader->setMat4("uf_xform_projection", xform_projection);
        
        if (item->textures_id.size() > 0) {
            std::string uf_text_prefix = "uf_text_";
            if (item->textures_id.size() == 1) {
                auto texture_id = item->textures_id.back();
                if (last_render_texture == "" ||  last_render_shader != texture_id) {
                    repository->textures.at(texture_id)->use();
                    current_shader->setInt(uf_text_prefix+std::to_string(0), 0);
                }
            } else {
                int i = 0;
                for (const auto texture_id: item->textures_id) {
                    repository->textures.at(texture_id)->use(i);
                    current_shader->setInt(uf_text_prefix+std::to_string(i), i);
                    ++i;
                }
            }
            last_render_shader = item->textures_id.back();
        }

        if (item->model_id == "") {
            LOG(ERROR) << "haven't load model yet, object id=\"" << item->id << "\"";
            exit(-1);
        }
        repository->models.at(item->model_id)->draw();
        // item->resetPosition();
    }   
}

}