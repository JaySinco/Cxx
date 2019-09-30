#pragma once
#include <vector>
#include "shader.h"

namespace cxx {

class Object {
public:
    Object(const std::string &name, const std::vector<float> &vertices, 
        const std::vector<unsigned> &indices);
    Object(const Object&) = delete;
    ~Object() {
        LOG(INFO) << "delete object, id=\"" << id << "\"";
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
    void draw();
private:
    unsigned VAO, VBO, EBO;
    int count;
    std::string id;
};

}