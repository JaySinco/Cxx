#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <glad/glad.h>
#include <algorithm>
#include <numeric>
#include "scenery.h"

namespace cxx {

Scenery::Scenery(const std::string &name): id(name) {}

Scenery::~Scenery() {
    LOG(INFO) << "delete scenery, id=\"" << id << "\"";
}

std::shared_ptr<Object> Scenery::getItemByName(const std::string &name) {
    return item_map.at(name);
}

std::shared_ptr<Object> Scenery::operator[](const std::string &name) {
    return getItemByName(name);
}

void Scenery::select(std::shared_ptr<Repository> repo) {
    repository = repo;
}

void Scenery::select(std::shared_ptr<Camera> ca) {
    camera = ca;
}

BoundRect Scenery::getBoundRect() const {
    BoundRect bounded;
    for (const auto &item_pair: item_map) {
        auto item = item_pair.second;
        auto rect = repository->models.at(item->model_id)->getBoundRect();
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
    for (const auto &item_pair: item_map) {
        auto item = item_pair.second;
        
        if (item->shader_id != "" && 
                (last_render_shader == "" || last_render_shader != item->shader_id)) {
            repository->shaders.at(item->shader_id)->use();
            last_render_shader = item->shader_id;
        }
        auto current_shader = repository->shaders.at(last_render_shader);
        current_shader->setMat4("uf_xform_model", item->getModelMatrix());
        current_shader->setMat4("uf_xform_view", camera->getViewMatrix());
        current_shader->setMat4("uf_xform_projection", camera->getProjectionMatrix());
        
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
    }   
}

}