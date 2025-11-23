#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
using namespace glm;

#include <cassert>

#include "../Info.hpp"
#include "../FrameGraph.hpp"
#include "DescriptorSetBuffer.hpp"

namespace MBG {

class TextureBuffer {
public:
    TextureBuffer(const TextureBufferParams& params) : size_(params.size) {
        // Generate buffer
        glGenBuffers(1, &buffer_id_);
        glBindBuffer(GL_TEXTURE_BUFFER, buffer_id_);

        uint format_byte_size = texture_format_byte_size[uint(params.format)];
        glBufferData(GL_TEXTURE_BUFFER, size_ * format_byte_size, params.data, GL_STATIC_DRAW);

        // Generate texture
        glGenTextures(1, &texture_id_);
        glBindTexture(GL_TEXTURE_BUFFER, texture_id_);
        assert(texture_id_ != 0);

        auto info = texture_formats[uint(params.format)];
        glTexBuffer(GL_TEXTURE_BUFFER, info.internal_format, buffer_id_);
    }

    ~TextureBuffer() {
        glDeleteTextures(1, &texture_id_);
        glDeleteBuffers(1, &buffer_id_);
    }

    uint getSize() const { return size_; }

protected:
    GLuint texture_id_ = 0;
    uint size_ = 0;

    friend class FrameGraph;
    friend class DescriptorSetBuffer;
private:
    GLuint buffer_id_ = 0;
};

}