#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
#include <glog/logging.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <opencv2/highgui/highgui.hpp>
#include "texture.h"

namespace cxx {

std::shared_ptr<Texture> Texture::fromFile(const std::string &name, 
        const std::string &path, bool flip) {
    return std::make_shared<Texture>(name, path, flip);    
}

Texture::Texture(const std::string &name, const std::string &path, bool flip): id(name) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); 
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
    if (flip)
        cv::flip(image, image, 0);
    if (!image.empty()) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.cols, image.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, image.data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        LOG(ERROR) << "failed to load texture, path=\"" << path << "\"";
        exit(-1);
    }
}

Texture::~Texture() {
    LOG(INFO) << "delete texture, id=\"" << id << "\"";
    glDeleteTextures(1, &texture);
}

void Texture::use(unsigned index) const { 
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, texture);
}

}
