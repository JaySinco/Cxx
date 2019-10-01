#pragma once
#include <vector>
#include <string>
#include <memory>

namespace cxx {

class Model {
public:
    enum ATTR { POS=3, COLOR=3, NORM=3, TEXT=2 };
    using VERTEX_DATA = std::vector<float>;
    using INDEX_DATA = std::vector<unsigned>;
    using ATTRIB_DATA = std::vector<ATTR>;
    static std::shared_ptr<Model> fromRawData(const std::string &name,
        const VERTEX_DATA &vertices, const INDEX_DATA &indices, const ATTRIB_DATA &attr);
    Model(const std::string &name, const VERTEX_DATA &vertices, 
        const INDEX_DATA &indices, const ATTRIB_DATA &attr);
    Model(const Model&) = delete;
    ~Model();
    void draw() const;
private:
    void load(const VERTEX_DATA &vertices, const INDEX_DATA &indices, const ATTRIB_DATA &attr);
    unsigned VAO, VBO, EBO;
    unsigned indices_count;
    std::string id;
};

}