#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <glad/glad.h>
#include <numeric>
#include "model.h"

namespace cxx {

Model::Model(
        const std::string &name,
        const std::vector<float> &vertices, 
        const std::vector<unsigned> &indices,
        const std::vector<VERTEX_ATTR> &attr): id(name) {
    load(vertices, indices, attr);
}

Model::~Model() {
    LOG(INFO) << "delete model, id=\"" << id << "\"";
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Model::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, 0);
}

void Model::load(
        const std::vector<float> &vertices,
        const std::vector<unsigned> &indices,
        const std::vector<VERTEX_ATTR> &attr) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*indices.size(), indices.data(), GL_STATIC_DRAW);
    indices_count = static_cast<unsigned>(indices.size());
    unsigned total_size = std::accumulate(attr.cbegin(), attr.cend(), 0);
    unsigned offset = 0;
    for (unsigned i = 0; i < attr.size(); ++i) {
        glVertexAttribPointer(i, attr[i], GL_FLOAT, GL_FALSE, 
            total_size * sizeof(float), (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += attr[i];
    }
    glBindVertexArray(0);
}

}
