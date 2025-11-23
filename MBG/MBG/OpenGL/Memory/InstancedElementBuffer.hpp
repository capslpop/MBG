#pragma once

#include "DescriptorSetBuffer.hpp"
#include "InstancedVertexBuffer.hpp"

namespace MBG {

class InstancedElementBuffer final : public InstancedVertexBuffer {
public:
    InstancedElementBuffer(const InstancedElementBufferParams& params)
        : InstancedVertexBuffer({
            params.vertex_attributes,
            params.attributes_count,
            params.instance_attributes,
            params.instance_attributes_count,
            params.vertex_data,
            params.vertex_count,
            params.instance_data,
            params.instance_count,
            params.buffer_usage,
            }),
        size_(params.element_count)
    {
        assert(params.element_count > 0);

        // Determine element byte size
        switch (params.element_attribute)
        {
        case ELEMENT_ATTRUBUTE::BYTE:    element_byte_size_ = 1; break;
        case ELEMENT_ATTRUBUTE::SHORT:   element_byte_size_ = 2; break;
        case ELEMENT_ATTRUBUTE::UINT32:  element_byte_size_ = 4; break;
        default: assert(false); break;
        }

        element_attrib_ = params.element_attribute;

        glBindVertexArray(vertex_array_id_);

        // Allocate and upload element buffer
        glGenBuffers(1, &element_buffer_id_);
        assert(element_buffer_id_ != 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_id_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            params.element_count * element_byte_size_,
            params.element_data,
            GLenum(params.buffer_usage));
    }

    virtual ~InstancedElementBuffer() override {
        glDeleteBuffers(1, &element_buffer_id_);
    }

protected:
    GLuint element_buffer_id_ = 0;
    ELEMENT_ATTRUBUTE element_attrib_{};
    uint element_byte_size_ = 0;
    uint size_ = 0;  // Number of elements

    friend class FrameGraph;
    friend class DescriptorSetBuffer;
};

}