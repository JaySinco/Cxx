#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <numeric>
#include "model.h"

namespace cxx {

std::shared_ptr<Model> Model::fromRawData(const std::string &name,
        const VERTEX_DATA &vertices, const INDEX_DATA &indices, const ATTRIB_DATA &attr) {
    return std::make_shared<Model>(name, vertices, indices, attr);
}

Model::Model(const std::string &name, const VERTEX_DATA &vertices, 
        const INDEX_DATA &indices, const ATTRIB_DATA &attr): id(name) {
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

void Model::load(const VERTEX_DATA &vertices, const INDEX_DATA &indices, 
        const ATTRIB_DATA &attr) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*indices.size(), indices.data(), GL_STATIC_DRAW);
    indices_count = indices.size();
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
