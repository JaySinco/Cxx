#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <glad/glad.h>
#include <algorithm>
#include <numeric>
#include "scenery.h"

namespace cxx {

namespace gl {

Scenery::Scenery(const std::string &name): id(name) {}

Scenery::~Scenery() {
    LOG(INFO) << "delete scenery, id=\"" << id << "\"";
}

std::shared_ptr<Object> Scenery::getObjectByName(const std::string &name) {
    return object_map.at(name);
}

BoundRect Scenery::getBoundRect() const {
    BoundRect bounded;
    for (const auto &object_pair: object_map) {
        auto object = object_pair.second;
        auto rect = storage->models.at(object->model_id)->getBoundRect();
        bounded.lowX = std::min(bounded.lowX, rect.lowX);
        bounded.maxX = std::max(bounded.maxX, rect.maxX);
        bounded.lowY = std::min(bounded.lowY, rect.lowY);
        bounded.maxY = std::max(bounded.maxY, rect.maxY);
        bounded.lowZ = std::min(bounded.lowZ, rect.lowZ);
        bounded.maxZ = std::max(bounded.maxZ, rect.maxZ);
    }
    LOG(INFO) << "scenery boundary acquired, rect=([" << bounded.lowX << ", " << bounded.maxX
        << "], [" << bounded.lowY << ", " << bounded.maxY << "], [" << bounded.lowZ 
        << ", " << bounded.maxZ << "]), id=\"" << id << "\"";
    return bounded;
}

void Scenery::render() {
    for (const auto &object_pair: object_map) {
        auto object = object_pair.second;
        // load shader
        if (object->shader_id != "" && 
                (last_render_shader == "" || last_render_shader != object->shader_id)) {
            LOG(INFO) << "using shader id=\"" << object->shader_id << "\"";
            storage->shaders.at(object->shader_id)->use();
            last_render_shader = object->shader_id;
        }
        auto current_shader = storage->shaders.at(last_render_shader);
        current_shader->setMat4("uf_xform_model", object->getModelMatrix());
        current_shader->setMat4("uf_xform_view", camera->getViewMatrix());
        current_shader->setMat4("uf_xform_projection", camera->getProjectionMatrix());
        // load material
        if (object->material_id.size() > 0) {
            auto material = storage->materials.at(object->material_id);
            storage->textures.at(material->diffuse_texture_id)->use(0);
            current_shader->setInt("uf_material.diffuse", 0);
            current_shader->setVec3("uf_material.specular", material->specularVec);
            current_shader->setFloat("uf_material.shininess", material->shininess);
        }
        // load camera && light
        current_shader->setVec3("uf_camera_pos", camera->posVec);
        current_shader->setVec3("uf_light.pos", light->posVec);
        current_shader->setVec3("uf_light.ambient", light->ambientVec);
        current_shader->setVec3("uf_light.diffuse", light->diffuseVec);
        current_shader->setVec3("uf_light.specular", light->specularVec);
        // load model
        if (object->model_id == "") {
            LOG(ERROR) << "haven't load model yet, object id=\"" << object->id << "\"";
            exit(-1);
        }
        storage->models.at(object->model_id)->draw();
    } 
};

} // namespace gl

} // namespace cxx