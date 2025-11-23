#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
using namespace glm;

#include "../Info.hpp"
#include "../FrameGraph.hpp"
#include "DescriptorSetBuffer.hpp"

namespace MBG {

class Texture2DBuffer {
public:
	Texture2DBuffer(const Texture2DBufferParams& params) : size_(params.size) {
		// Generate and bind texture
		glGenTextures(1, &texture_id_);
		glBindTexture(GL_TEXTURE_2D, texture_id_);
		assert(texture_id_ != 0);

		// Set default texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GLenum(params.min_filter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GLenum(params.mag_filter));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GLenum(params.wrap_s));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GLenum(params.wrap_t));

		setUnpackAlignment(params.format, params.size.x);

		auto info = texture_formats[uint(params.format)]; // grab the correct default format data
		glTexImage2D(GL_TEXTURE_2D, 0, info.internal_format, size_.x, size_.y, 0, info.format, info.type, params.data);

		if (params.generate_mipmaps)
			glGenerateMipmap(GL_TEXTURE_2D);
	}

	~Texture2DBuffer() {
		glDeleteTextures(1, &texture_id_);
	}

	uvec2 getSize() const { return size_; }

protected:
	GLuint texture_id_ = 0;
	uvec2 size_;

	friend class FrameGraph;
	friend class DescriptorSetBuffer;
};

}