#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <vector>
#include "object.h"
#include "camera.h"

namespace cxx {

class Scenery {
public:
    Scenery(const std::string &name);
    ~Scenery();
    template<typename... Args>
    std::shared_ptr<Object> newItem(const std::string &name, Args&&... args) {
        if (item_map.find(name) != item_map.end()) {
            LOG(ERROR) << "can't add new item(" << "id=\"" << name 
                << "\"), already exists in scenery(" << "id=\"" << id << "\""; 
            exit(-1);
        }
        auto item = std::make_shared<Object>(name, args...);
        item_map[name] = item;
        return item;
    }
    std::shared_ptr<Object> getItemByName(const std::string &name);
    std::shared_ptr<Object> operator[](const std::string &name);
    void select(std::shared_ptr<Repository> repo);
    void select(std::shared_ptr<Camera> ca);
    void render();
    BoundRect getBoundRect() const;
    std::shared_ptr<Repository> repository;
    std::shared_ptr<Camera> camera;
    const std::string id;
private:
    std::map<std::string, std::shared_ptr<Object>> item_map;
    std::string last_render_shader;
    std::string last_render_texture;
};

}