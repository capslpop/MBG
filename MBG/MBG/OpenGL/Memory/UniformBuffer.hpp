#pragma once

#include <glad/glad.h>

#include "../Info.hpp"
#include "../FrameGraph.hpp"
#include "DescriptorSetBuffer.hpp"

//------------------------------------------------------------
// UNIFORM BUFFER OBJECT
//	This creates a uniform buffer struct
//------------------------------------------------------------

namespace MBG {

class UniformBuffer {
public:
	UniformBuffer(const UniformBufferParams& params) : size_(params.size) {
		assert(size_ > 0);
		glGenBuffers(1, &uniform_id_);
		glBindBuffer(GL_UNIFORM_BUFFER, uniform_id_);
		glBufferData(GL_UNIFORM_BUFFER, params.size, params.data, GLenum(params.buffer_usage));
	}

	size_t getSize() const { return size_; }

protected:
	GLuint uniform_id_;
	size_t size_;

	friend class FrameGraph;
	friend class DescriptorSetBuffer;
};

}