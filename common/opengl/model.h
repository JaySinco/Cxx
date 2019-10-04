#pragma once
#include <vector>
#include <string>
#include <memory>

namespace cxx {

struct Cuboid {
    float lowX = std::numeric_limits<float>::max();
    float lowY = std::numeric_limits<float>::max();
    float lowZ = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();
    float maxZ = std::numeric_limits<float>::lowest();
};

class Model {
public:
    enum VERTEX_ATTR { POS=3, COLOR=3, NORM=3, TXR=2 };
    Model(
        const std::string &name,
        const std::vector<float> &vertices, 
        const std::vector<unsigned> &indices,
        const std::vector<VERTEX_ATTR> &attr);
    Model(const std::string &name, const std::string &path);
    Model(const Model&) = delete;
    ~Model();
    void draw() const;
    Cuboid getBoundCuboid() const;
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
    Cuboid bound_cuboid;
    unsigned VAO, VBO, EBO;
    unsigned indices_count;
    std::string id;
};

}