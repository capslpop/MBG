#pragma once

//------------------------------------------------------------
// VERTEX AND ELEMNET BUFFER
//	This class creates a vertex buffer and element buffer then
//  defines its structure
//------------------------------------------------------------

#include "VertexBuffer.hpp"

class VertexElementBuffer
{
public:
	VertexElementBuffer(VertexBuffer& vertex_buffer, void* element_buffer_data,	GLsizeiptr element_buffer_size, std::string element_buffer_name) 
		: m_vertex_buffer(vertex_buffer),
		Name(element_buffer_name)
	{
		assert(element_buffer_data != nullptr);
		assert(element_buffer_size > 0);

		m_vertex_buffer.Bind();
		
		glGenBuffers(1, &m_element_buffer_object);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer_object);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_buffer_size, element_buffer_data, m_vertex_buffer.Usage);

#ifdef _DEBUG
		std::cerr << "VertexElementBuffer::VertexElementBuffer(VertexBuffer&, void*, GLsizeiptr): " << Name << " changed the vertex array object of " << m_vertex_buffer.Name << "." << std::endl;
#endif

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	~VertexElementBuffer() {
		glDeleteBuffers(1, &m_element_buffer_object);
#ifdef _DEBUG
		std::cerr << "VertexElementBuffer::~VertexElementBuffer: <Reminder> " << Name << " did NOT delete " << m_vertex_buffer.Name << "'s vertex array object." << std::endl;
#endif	
	}

	inline void Bind()
	{
		m_vertex_buffer.Bind();
	}

	const std::string Name;

private:
	VertexBuffer& m_vertex_buffer;
	GLuint m_element_buffer_object;
};

