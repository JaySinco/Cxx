#pragma once
#include <vector>
#include "base.h"

namespace cxx
{

namespace gl
{

struct BoundRect
{
    float lowX = std::numeric_limits<float>::max();
    float lowY = std::numeric_limits<float>::max();
    float lowZ = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();
    float maxZ = std::numeric_limits<float>::lowest();

    void applyModelMatrix(const glm::mat4 &xform_model);
};

std::ostream &operator<<(std::ostream &out, const BoundRect &rect);

class Model : public Base
{
public:
    enum VERTEX_ATTR
    {
        POS = 3,
        COLOR = 3,
        NORM = 3,
        TXR = 2
    };
    Model(
        const std::string &id,
        const std::vector<float> &vertices,
        const std::vector<unsigned> &indices,
        const std::vector<VERTEX_ATTR> &attr);
    Model(const std::string &id, const std::string &path);
    ~Model();
    void draw() const;
    BoundRect getBoundRect() const;

private:
    void load(
        const std::vector<float> &vertices,
        const std::vector<unsigned> &indices,
        const std::vector<VERTEX_ATTR> &attr);
    void readObjFile(
        const std::string &path,
        std::vector<float> &vertices,
        std::vector<unsigned> &indices,
        std::vector<VERTEX_ATTR> &attr);
    BoundRect bound_rect;
    unsigned VAO, VBO, EBO;
    unsigned indices_count;
};

} // namespace gl

} // namespace cxx