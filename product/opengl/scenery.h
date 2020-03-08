#pragma once
#include <set>
#include "base.h"
#include "camera.h"
#include "light.h"
#include "material.h"
#include "model.h"
#include "object.h"
#include "shader.h"
#include "storage.h"
#include "texture.h"

namespace cxx
{

namespace gl
{

class Scenery : public Base
{
public:
    Scenery(const std::string &id);
    void putObject(const std::string &id);
    void putCamera(const std::string &id);
    void putLight(const std::string &id);
    BoundRect getBoundRect(const Storage &storage) const;
    void render(const Storage &storage);

private:
    std::set<std::string> object_set;
    std::string camera_;
    std::string light_;
    std::string last_render_shader;
};

} // namespace gl

} // namespace cxx