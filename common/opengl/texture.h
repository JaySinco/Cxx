#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace cxx {

class Texture {
public:
    Texture(const std::string &name, const std::string &path, bool flip = false);
    Texture(const Texture&) = delete;
    ~Texture() {
        LOG(INFO) << "delete texture, id=\"" << id << "\"";
        glDeleteTextures(1, &texture);
    }
    void use(unsigned index) const { 
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
private:
    unsigned texture;
    std::string id;
};

}