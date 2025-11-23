#pragma once

//------------------------------------------------------------
// VERTICES BUFFER
//	This class creates a vertex buffer and defines its structure
//------------------------------------------------------------

#include <glad/glad.h>

#include "../Info.hpp"
#include "../FrameGraph.hpp"
#include "DescriptorSetBuffer.hpp"

namespace MBG {

class VertexBuffer {
public:
	VertexBuffer(const VertexBufferParams& params) : size_(params.count) {
		assert(params.count > 0);
		
		struct_size_ = 0; // Total number of bytes in the struct
		for (unsigned i = 0; i < params.attributes_count; i++) {
			Attributes a = params.attributes[i];
			struct_size_ += getAttributeSize(a.attribute) * a.count;
		}

		glGenVertexArrays(1, &vertex_array_id_);
		glBindVertexArray(vertex_array_id_);
		assert(vertex_array_id_ != 0);
	
		glGenBuffers(1, &vertex_buffer_id_);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id_);
		glBufferData(GL_ARRAY_BUFFER, params.count * struct_size_, params.data, GLenum(params.buffer_usage));
		assert(vertex_buffer_id_ != 0);
		
		size_t byte_offset = 0;
		for (unsigned i = 0; i < params.attributes_count; i++) {
			glEnableVertexAttribArray(i);

			Attributes a = params.attributes[i];

			// Check the default version the data should be interpreted as
			if (isFloat(a.attribute)) { 
				glVertexAttribPointer(i, a.count, GLenum(a.attribute), GL_FALSE, struct_size_, (void*)byte_offset);
			}
			else {
				glVertexAttribIPointer(i, a.count, GLenum(a.attribute), struct_size_, (void*)byte_offset);
			}

			byte_offset += getAttributeSize(a.attribute) * a.count;
		}
	}

	virtual ~VertexBuffer() {
		glDeleteVertexArrays(1, &vertex_array_id_);
		glDeleteBuffers(1, &vertex_buffer_id_);
	}
	
protected:
	GLuint vertex_array_id_;
	GLuint vertex_buffer_id_;
	uint size_; // number of vertices
	uint struct_size_;

	friend class FrameGraph;
	friend class DescriptorSetBuffer;
};

}