#include "Utils.h"

#include <iostream>

#include "stb_image.h"
#include "backends/imgui_impl_opengl3_loader.h"

namespace Mio {
    GLuint Utils::LoadTexture(const char* filename) {
        int width, height, channels;
        unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
        if (!data) {
            std::cerr << "Failed to load image: " << filename << std::endl;
            return 0;
        }

        GLuint texture_id;
        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(data);
        return texture_id;
    }
}
