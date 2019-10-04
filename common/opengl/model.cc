#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <glad/glad.h>
#include <numeric>
#include <algorithm>
#include <array>
#include <map>
#include <fstream>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "model.h"

namespace cxx {

Model::Model(
        const std::string &name,
        const std::vector<float> &vertices, 
        const std::vector<unsigned> &indices,
        const std::vector<VERTEX_ATTR> &attr): id(name) {
    load(vertices, indices, attr);
}

Model::Model(const std::string &name, const std::string &path): id(name) {
    std::string prefix = path.substr(path.rfind("."));
    std::vector<float> vertices;
    std::vector<unsigned> indices;
    std::vector<VERTEX_ATTR> attr;
    if (prefix == ".obj") {
        readObjFile(path, vertices, indices, attr);
    } else {
        LOG(ERROR) << "unsupported model file suffix name, path=\"" << path << "\"";
        exit(-1);
    }
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

BoundRect Model::getBoundRect() const {
    return bound_rect;
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

struct IndexKey {
    int vertexIndex;
    int normalIndex;
    int texcoordIndex;
    bool operator<(const IndexKey &other) const {
        return (vertexIndex < other.vertexIndex) ||
            (normalIndex < other.normalIndex) || (texcoordIndex < other.texcoordIndex);
    }
};

void Model::readObjFile(
        const std::string &path, 
        std::vector<float> &vertices,
        std::vector<unsigned> &indices,
        std::vector<VERTEX_ATTR> &attr) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str());
    if (!ret || shapes.size() <= 0) {
        LOG(ERROR) << "failed to load .obj model, path=\""
            << path << "\", err=\"" << err << "\"";
        exit(-1);
    }
    if (shapes.size() > 1)
        LOG(WARNING) << "aggregate " << shapes.size() << " shapes into one";

    if (attrib.vertices.size() > 0)  attr.push_back(POS);
    if (attrib.texcoords.size() > 0) attr.push_back(TXR);
    if (attrib.normals.size() > 0) attr.push_back(NORM);

    LOG(INFO) << "model loaded, path=\"" << path << "\", stat="
        << "(#v" << attrib.vertices.size() << " #vn" << attrib.normals.size()  
        << " #vt" << attrib.texcoords.size()
        << " #f" << shapes[0].mesh.num_face_vertices.size() << ")"
        << "\", id=\"" << id << "\"";

    unsigned indiceIndex = 0;
    for (size_t s = 0; s < shapes.size(); s++) {
        std::map<IndexKey, unsigned> speedUp;
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];
            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                IndexKey key = { idx.vertex_index, idx.normal_index, idx.texcoord_index };
                if (speedUp.find(key) != speedUp.end()) {
                    indices.push_back(speedUp[key]);
                    continue;
                }
                if (attrib.vertices.size() > 0) {
                    tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
                    tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
                    tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
                    bound_rect.lowX = std::min(bound_rect.lowX, vx);
                    bound_rect.maxX = std::max(bound_rect.maxX, vx);
                    bound_rect.lowY = std::min(bound_rect.lowY, vy);
                    bound_rect.maxY = std::max(bound_rect.maxY, vy);
                    bound_rect.lowZ = std::min(bound_rect.lowZ, vz);
                    bound_rect.maxZ = std::max(bound_rect.maxZ, vz);
                    vertices.insert(vertices.end(), { vx, vy, vz });
                }
                if (attrib.texcoords.size() > 0) {
                    tinyobj::real_t tx = attrib.texcoords[2*idx.texcoord_index+0];
                    tinyobj::real_t ty = attrib.texcoords[2*idx.texcoord_index+1];
                    vertices.insert(vertices.end(), { tx, ty });
                }
                if (attrib.normals.size() > 0) {
                    tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
                    tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
                    tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];
                    vertices.insert(vertices.end(), { nx, ny, nz });
                }
                indices.push_back(indiceIndex);
                speedUp[key] = indiceIndex++;
            }
            index_offset += fv;
        }
    }
}

}
