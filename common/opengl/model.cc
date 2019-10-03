#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <glad/glad.h>
#include <numeric>
#include <algorithm>
#include <array>
#include <map>
#include <fstream>
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

void Model::readObjFile(
        const std::string &path, 
        std::vector<float> &vertices,
        std::vector<unsigned> &indices,
        std::vector<VERTEX_ATTR> &attr) {
    std::ifstream modelFile(path);
    if (!modelFile) {
        LOG(ERROR) << "failed to load .obj model, path=\"" << path << "\"";
        exit(-1);
    }
    std::vector<std::array<float, 3>> raw_v;
    std::vector<std::array<float, 2>> raw_vt;
    std::vector<std::array<float, 3>> raw_vn;
    std::map<std::string, int> speedUp;
    std::string line;
    int indiceIndex = 0;
    int faceCount = 0;
    bool meetF = false;  
    while (std::getline(modelFile, line)) {
        std::istringstream iss(line);
        char trash;
        if (line.substr(0, 2) == "v ") {
            iss >> trash;
            std::array<float, 3> v;
            for (int i = 0; i<3; ++i) 
                iss >> v[i];
            raw_v.push_back(v);
        }
        else if (line.substr(0, 2) == "vn") {
            iss >> trash >> trash;
            std::array<float, 3> vn;
            for (int i = 0; i < 3; ++i)
                iss >> vn[i];
            raw_vn.push_back(vn);
        }
        else if (line.substr(0, 2) == "vt") {
            iss >> trash >> trash;
            std::array<float, 2> vt;
            for (int i = 0; i<2; ++i)
                iss >> vt[i];
            raw_vt.push_back(vt);
        }
        else if (line.substr(0, 2) == "f ") {
            ++faceCount;
            if (!meetF) {
                if (raw_v.size() > 0)  attr.push_back(POS);
                if (raw_vt.size() > 0) attr.push_back(TXR);
                if (raw_vn.size() > 0) attr.push_back(NORM);
                meetF = true;
            }
            iss.get(trash).get(trash);
            std::string segment, number;
            std::vector<std::string> vStr;
            while (std::getline(iss, segment, ' ')) {
                if (speedUp.find(segment) != speedUp.end()) {
                    indices.push_back(speedUp[segment]);
                    continue;
                }
                std::istringstream isg(segment);
                int i = 0;
                while (std::getline(isg, number, '/')) {
                    if (number.size() > 0) {
                        int index = atoi(number.c_str())-1;
                        if (i == 0) {
                            auto &value = raw_v.at(index);
                            std::copy(value.cbegin(), value.cend(), std::back_inserter(vertices));
                        } else if (i == 1) {
                            auto &value = raw_vt.at(index);
                            std::copy(value.cbegin(), value.cend(), std::back_inserter(vertices));
                        } else if (i == 2) {
                            auto &value = raw_vn.at(index);
                            std::copy(value.cbegin(), value.cend(), std::back_inserter(vertices));
                        } else {
                            LOG(ERROR) << "abnormal data, segment=\"" << segment << "\", path=\""
                                << path << "\"";
                            exit(-1);
                        }
                    }
                    ++i;
                }
                indices.push_back(indiceIndex);
                speedUp[segment] = indiceIndex++;
            }
        }
    }
    LOG(INFO) << "model loaded from \"" << path <<  "\" (#v" << raw_v.size()
        << " #vn" << raw_vn.size()  << " #vt" << raw_vt.size() << " #f" << faceCount << ")";
}

}
