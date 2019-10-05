#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <vector>
#include "object.h"
#include "camera.h"
#include "gizmo.h"

namespace cxx {

namespace gl {

class Scenery {
public:
    Scenery(const std::string &name);
    ~Scenery();
    template<typename... Args>
    std::shared_ptr<Storage> newStorage(const std::string &name, Args&&... args) {
        storage = std::make_shared<Storage>(name, args...);
        return storage;
    }
    template<typename... Args>
    std::shared_ptr<Object> putObject(const std::string &name, Args&&... args) {
        auto object = std::make_shared<Object>(name, args...);
        object_map[name] = object;
        return object;
    }
    template<typename... Args>
    std::shared_ptr<Camera> putCamera(const std::string &name, Args&&... args) {
        camera = std::make_shared<Camera>(name, args...);
        return camera;
    }
    template<typename... Args>
    std::shared_ptr<Light> putLight(const std::string &name, Args&&... args) {
        light = std::make_shared<Light>(name, args...);
        return light;
    }
    std::shared_ptr<Object> getObjectByName(const std::string &name);
    BoundRect getBoundRect() const;
    void render();
    std::shared_ptr<Storage> storage;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Light> light;
    const std::string id;
private:
    std::map<std::string, std::shared_ptr<Object>> object_map;
    std::string last_render_shader;
};

} // namespace gl

} // namespace cxx