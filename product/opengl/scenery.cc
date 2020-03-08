#include "scenery.h"
#include <algorithm>
#include <numeric>

namespace cxx {

namespace gl {

    Scenery::Scenery(const std::string& id)
        : Base(Base::SCENERY, id)
    {
    }

    void Scenery::putObject(const std::string& id)
    {
        object_set.insert(id);
    }

    void Scenery::putCamera(const std::string& id)
    {
        camera_ = id;
    }

    void Scenery::putLight(const std::string& id)
    {
        light_ = id;
    }

    BoundRect Scenery::getBoundRect(const Storage& storage) const
    {
        BoundRect bounded;
        for (const auto& object_id : object_set) {
            auto object = storage.get<Object>(object_id);
            auto rect = storage.get<Model>(object->model())->getBoundRect();
            rect.applyModelMatrix(object->getModelMatrix());
            bounded.lowX = std::min(bounded.lowX, rect.lowX);
            bounded.maxX = std::max(bounded.maxX, rect.maxX);
            bounded.lowY = std::min(bounded.lowY, rect.lowY);
            bounded.maxY = std::max(bounded.maxY, rect.maxY);
            bounded.lowZ = std::min(bounded.lowZ, rect.lowZ);
            bounded.maxZ = std::max(bounded.maxZ, rect.maxZ);
        }
        LOG(INFO) << *this << " boundary obtained, rect=" << bounded;
        return bounded;
    }

    void Scenery::render(const Storage& storage)
    {
        auto camera = storage.get<Camera>(camera_);
        auto light = storage.get<Light>(light_);
        for (const auto& object_id : object_set) {
            auto object = storage.get<Object>(object_id);
            // load shader
            if (object->shader() != "" && (last_render_shader == "" || last_render_shader != object->shader())) {
                auto shader = storage.get<Shader>(object->shader());
                LOG(INFO) << "using " << *shader;
                shader->use();
                last_render_shader = shader->getId();
            }
            auto current_shader = storage.get<Shader>(last_render_shader);
            current_shader->setMat4("uf_xform_model", object->getModelMatrix());
            // load material
            if (object->material() != "") {
                auto material = storage.get<Material>(object->material());
                storage.get<Texture>(material->diff_texture())->use(0);
                current_shader->setInt("uf_material.diffuse", 0);
                current_shader->setVec3("uf_material.specular", material->specular());
                current_shader->setFloat("uf_material.shininess", material->shininess());
            }
            // load camera
            current_shader->setVec3("uf_camera_pos", camera->pos());
            current_shader->setMat4("uf_xform_view", camera->getViewMatrix());
            current_shader->setMat4("uf_xform_projection", camera->getProjectionMatrix());
            // load light
            current_shader->setInt("uf_light.all_type", light->data().all_type);
            current_shader->setVec3("uf_light.all_ambient", light->data().all_ambient);
            current_shader->setVec3("uf_light.all_diffuse", light->data().all_diffuse);
            current_shader->setVec3("uf_light.all_specular", light->data().all_specular);
            current_shader->setVec3("uf_light.point_spot_position", light->data().point_spot_position);
            current_shader->setVec3("uf_light.direct_spot_direction", light->data().direct_spot_direction);
            current_shader->setFloat("uf_light.point_constant", light->data().point_constant);
            current_shader->setFloat("uf_light.point_linear", light->data().point_linear);
            current_shader->setFloat("uf_light.point_quadratic", light->data().point_quadratic);
            current_shader->setFloat("uf_light.spot_innerCutOff", light->data().spot_innerCutOff);
            current_shader->setFloat("uf_light.spot_outerCutOff", light->data().spot_outerCutOff);
            // load model
            if (object->model() == "") {
                LOG(ERROR) << object << " haven't load model yet";
                exit(-1);
            }
            storage.get<Model>(object->model())->draw();
        }
    };

} // namespace gl

} // namespace cxx