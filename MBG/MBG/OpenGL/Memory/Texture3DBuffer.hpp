#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
using namespace glm;

#include "../Info.hpp"
#include "../FrameGraph.hpp"
#include "DescriptorSetBuffer.hpp"

namespace MBG {

class Texture3DBuffer {
public:
	Texture3DBuffer(const Texture3DBufferParams& params) : size_(params.size) {
		// Generate and bind texture
		glGenTextures(1, &texture_id_);
		glBindTexture(GL_TEXTURE_3D, texture_id_);
		assert(texture_id_ != 0);

		// Set default texture parameters
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GLenum(params.min_filter));
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GLenum(params.mag_filter));
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GLenum(params.wrap_s));
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GLenum(params.wrap_t));
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GLenum(params.wrap_r));

		setUnpackAlignment(params.format, params.size.x);

		auto info = texture_formats[uint(params.format)]; // grab the correct default format data
		glTexImage3D(GL_TEXTURE_3D, 0, info.internal_format, size_.x, size_.y, size_.z, 0, info.format, info.type, params.data);

		if (params.generate_mipmaps)
			glGenerateMipmap(GL_TEXTURE_3D);
	}

	~Texture3DBuffer() {
		glDeleteTextures(1, &texture_id_);
	}

	uvec3 getSize() const { return size_; }

protected:
	GLuint texture_id_ = 0;
	uvec3 size_;

	friend class FrameGraph;
	friend class DescriptorSetBuffer;
};

}