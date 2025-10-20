#pragma once

//------------------------------------------------------------
// VERTEX AND ELEMNET BUFFER
//	This class creates a vertex buffer and element buffer then
//  defines its structure
//------------------------------------------------------------

#include "VertexBuffer.hpp"

class VertexElementBuffer final : public VertexBuffer
{
public:
	VertexElementBuffer(void* vertex_buffer_data, GLsizeiptr vertex_buffer_size, void* element_buffer_data, GLsizeiptr element_buffer_size, std::string buffer_name, GLenum usage) 
		: VertexBuffer(vertex_buffer_size, buffer_name, vertex_buffer_data, usage)
	{
		assert(element_buffer_data != nullptr);
		assert(element_buffer_size > 0);

		glBindVertexArray(m_vertex_array_object);
		
		glGenBuffers(1, &m_element_buffer_object);
		assert(m_element_buffer_object != 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer_object);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_buffer_size, element_buffer_data, m_usage);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	virtual ~VertexElementBuffer() override {
		glDeleteBuffers(1, &m_element_buffer_object);
	}

private:
	GLuint m_element_buffer_object;
};

