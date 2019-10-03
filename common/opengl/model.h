#pragma once
#include <vector>
#include <string>
#include <memory>

namespace cxx {

class Model {
public:
    enum VERTEX_ATTR { POS=3, COLOR=3, NORM=3, TXR=2 };
    Model(
        const std::string &name,
        const std::vector<float> &vertices, 
        const std::vector<unsigned> &indices,
        const std::vector<VERTEX_ATTR> &attr);
    Model(const Model&) = delete;
    ~Model();
    void draw() const;
private:
    void load(
        const std::vector<float> &vertices,
        const std::vector<unsigned> &indices,
        const std::vector<VERTEX_ATTR> &attr);
    unsigned VAO, VBO, EBO;
    unsigned indices_count;
    std::string id;
};

}