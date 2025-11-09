
#pragma once

//------------------------------------------------------------
// VERTICES BUFFER
//	This class creates a vertex buffer and defines its structure
//------------------------------------------------------------

#include <cassert>
#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>

class VertexBuffer
{
public:
	VertexBuffer(GLsizeiptr buffer_size, std::string buffer_name = "vertex_buffer", void* data = nullptr, GLenum buffer_usage = GL_STATIC_DRAW)
		: m_buffer_size(buffer_size)
		, Name(buffer_name)
		, m_total_attribute_count(0)
		, m_vertex_array_object(0)
		, m_vertex_buffer_object(0)
		, m_stride(0)
		, m_usage(buffer_usage)
	{
		assert(buffer_size > 0);
		assert(data != nullptr);

		glGenVertexArrays(1, &m_vertex_array_object);
		glBindVertexArray(m_vertex_array_object);
	
		glGenBuffers(1, &m_vertex_buffer_object);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_object);
		glBufferData(GL_ARRAY_BUFFER, buffer_size, data, buffer_usage);
		
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		assert(m_vertex_buffer_object != 0);
		assert(m_vertex_array_object != 0);
	}

	void BeginAttrib()
	{
		assert(m_vertex_array_object != 0);
		glBindVertexArray(m_vertex_array_object);

		for (GLuint i = 0; i < m_total_attribute_count; ++i)
		{
			glDisableVertexAttribArray(i);
		}

		m_total_attribute_count = 0;
		m_stride = 0;
		assert(m_total_attribute_count == 0);
		assert(m_stride == 0);

#ifdef _DEBUG
		const size_t attribute_size_count = m_attribute_count.size();
		const size_t attribute_type_count = m_attribute_type.size();
		const size_t attribute_pointer_count = m_attribute_pointer.size();
		assert(attribute_size_count == 0);
		assert(attribute_type_count == 0);
		assert(attribute_pointer_count == 0);
#endif

		glBindVertexArray(0);
	}

	void EndAttrib()
	{
		assert(m_vertex_array_object != 0);

		glBindVertexArray(m_vertex_array_object);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_object);

		std::cerr << "VertexBuffer::EndAttrib() is running." << std::endl;

		for (GLuint attribute_index = 0; attribute_index < m_total_attribute_count; ++attribute_index)
		{
			glEnableVertexAttribArray(attribute_index);
			glVertexAttribPointer(attribute_index, m_attribute_count[attribute_index], m_attribute_type[attribute_index], GL_FALSE, m_stride, reinterpret_cast<void*>(m_attribute_pointer[attribute_index]));
#ifdef _DEBUG
			std::cerr << "VertexBuffer::EndAttrib(): glVertexAttribPointer(" << attribute_index << ", " << m_attribute_count[attribute_index] << ", " << m_attribute_type[attribute_index] << ", GL_FALSE, " << m_stride << ", reinterpret_cast<void*>(" << m_attribute_pointer[attribute_index] << "));" << std::endl;
#endif
		}

		std::cerr << "VertexBuffer::EndAttrib() completed." << std::endl;

		m_attribute_count.clear();
		m_attribute_type.clear();
		m_attribute_pointer.clear();

#ifdef _DEBUG
		const size_t attribute_size_count = m_attribute_count.size();
		const size_t attribute_type_count = m_attribute_type.size();
		const size_t attribute_pointer_count = m_attribute_pointer.size();
		assert(attribute_size_count == 0);
		assert(attribute_type_count == 0);
		assert(attribute_pointer_count == 0);
#endif

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void AddFloat(GLuint count)
	{
		assert(count > 0);

		m_attribute_count.push_back(count);
		m_attribute_type.push_back(GL_FLOAT);
		m_attribute_pointer.push_back(m_stride);
		m_stride += sizeof(GLfloat) * count;

		++m_total_attribute_count;

#ifdef _DEBUG
		const size_t attribute_size_count = m_attribute_count.size();
		const size_t attribute_type_count = m_attribute_type.size();
		const size_t attribute_pointer_count = m_attribute_pointer.size();
		assert(attribute_size_count == m_total_attribute_count);
		assert(attribute_size_count == attribute_type_count);
		assert(attribute_size_count == attribute_pointer_count);
#endif
	}

	virtual ~VertexBuffer()
	{
		glDeleteBuffers(1, &m_vertex_buffer_object);
		glDeleteVertexArrays(1, &m_vertex_array_object);
	}

	inline GLsizeiptr GetVertexBufferSize() const
	{
		return m_buffer_size;
	}

	inline void Bind()
	{
		assert(m_vertex_array_object != 0);
		glBindVertexArray(m_vertex_array_object);
	}

	/*
	inline GLuint GetVBOIndex() const
	{
		return m_vertex_buffer_object;
	}

	inline GLsizei GetStride() const
	{
		return m_stride;
	}

	inline GLuint GetVAOIndex() const
	{
		return m_vertex_array_object;
	}
	*/

	const std::string Name;

protected:
	GLsizeiptr m_buffer_size;
	GLuint m_vertex_buffer_object;

	GLuint m_vertex_array_object;
	GLsizei m_stride;

	GLenum m_usage;

	GLuint m_total_attribute_count;
	std::vector<GLint> m_attribute_count;
	std::vector<GLenum> m_attribute_type;
	std::vector<GLuint> m_attribute_pointer;
};