#pragma once

//------------------------------------------------------------
// VERTEX AND ELEMNET BUFFER
//	This class creates a vertex buffer and element buffer then
//  defines its structure
//------------------------------------------------------------

#include "VertexBuffer.hpp"
#include "../Info.hpp"
#include "../FrameGraph.hpp"
#include "DescriptorSetBuffer.hpp"

namespace MBG {

class ElementBuffer final : public VertexBuffer {
public:
	ElementBuffer(ElementBufferParams params)
		: VertexBuffer({params.vertex_attributes, params.attributes_count, params.vertex_data, params.vertex_count, params.buffer_usage}), size_(params.element_count)
	{
		assert(params.element_count > 0);

		element_attrib_ = params.element_attribute;

		// Now we need to get the number of bytes the element is using
		size_t element_byte_size_ = 0;
		switch (params.element_attribute) {
			case ELEMENT_ATTRUBUTE::BYTE: element_byte_size_ = 1; break;
			case ELEMENT_ATTRUBUTE::SHORT: element_byte_size_ = 2; break;
			case ELEMENT_ATTRUBUTE::UINT32: element_byte_size_ = 4; break;
			default: assert(false); break; // There is an unknown type
		}
		
		glGenBuffers(1, &element_buffer_id_);
		assert(element_buffer_id_ != 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, params.element_count * element_byte_size_, params.element_data, GLenum(params.buffer_usage));
	}

	virtual ~ElementBuffer() override {
		glDeleteBuffers(1, &element_buffer_id_);
	}

protected:
	GLuint element_buffer_id_;
	ELEMENT_ATTRUBUTE element_attrib_; // we need this for drawing since openGL never can seam to keep things consistant
	uint element_byte_size_;
	uint size_; // number of elements

	friend class FrameGraph;
	friend class DescriptorSetBuffer;
};

}