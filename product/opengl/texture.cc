#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"
#include "stb_image.h"

namespace cxx {

namespace gl {

    Texture::Texture(
        const std::string& id,
        const std::string& path,
        bool flip_y_aixs)
        : Base(Base::TEXTURE, id)
    {

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(flip_y_aixs);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            LOG(ERROR) << "failed to load texture, path=" << QUOT(path);
            exit(-1);
        }
        stbi_image_free(data);
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &texture);
    }

    void Texture::use(unsigned index) const
    {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

} // namespace gl

} // namespace cxx