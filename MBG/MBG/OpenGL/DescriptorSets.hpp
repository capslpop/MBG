#pragma once

#include <cassert>
#include <cstring>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>
#include "FrameGraph.hpp"
#include "Texture2D.hpp"
#include "VertexElementBuffer.hpp"

class UniformDescriptor
{
public:
	UniformDescriptor(std::string name, std::string type, uint8_t* val, size_t val_size)
	{
		assert(!name.empty() && type == "3f");
		assert(val != nullptr && val_size > 0);
		uniform_name = name;
		uniform_type = type;
		uniform_val = new uint8_t[val_size];
		memcpy(uniform_val, val, val_size);
	}

	~UniformDescriptor()
	{
		delete uniform_val;
	}

	inline void SetName(const std::string name)
	{
		assert(!name.empty());
		uniform_name = name;
	}

	inline void SetType(const std::string type)
	{
		assert(type == "3f");
		uniform_type = type;
	}

	inline void SetVal(uint8_t* val, size_t new_val_size)
	{
		assert(val != nullptr && new_val_size > 0);
		delete uniform_val;
		uniform_val = new uint8_t[new_val_size];// could use smart pointers, but I dont have experience with them, maybe someone else.
		memcpy(uniform_val, val, new_val_size);
		val_size = new_val_size;
	}

private:
	std::string uniform_name;
	std::string uniform_type;
	uint8_t* uniform_val = nullptr;
	size_t val_size;
};

class UniformBlockDescriptor
{
public:
	UniformBlockDscriptor(const void* data, size_t size)
		: m_size(size)
	{
		assert(data != nullptr && size > 0);
		glGenBuffers(1, &m_uniform_block);
		glBindBuffer(GL_UNIFORM_BUFFER, m_uniform_block);
		glBufferData(GL_UNIFORM_BUFFER, size, data, GL_STATIC_DRAW);

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

private:
	GLuint m_uniform_block;
	size_t m_size;
};

class DescriptorSet
{
public:
	friend class FrameGraph;

	DescriptorSet() = default;

	inline void AddFramebufferTexture(const Texture2D& framebuffer_texture)
	{
		m_framebuffer_textures.push_back(&framebuffer_texture);
	}

	inline void AddVertexBuffer(const VertexBuffer& vertex_buffer)
	{
		m_vertex_buffers.push_back(&vertex_buffer);
	}

	inline void AddVertexElementBuffer(const VertexElementBuffer& vertex_buffer)
	{
		m_vertex_buffers.push_back(&vertex_element_buffer);
	}

	void AddUniformDescriptor(UniformDescriptor uniform_descriptor)
	{
		assert(!(uniform_descriptor.GetName().empty() || uniform_descriptor.GetType().empty()) && (uniform_descriptor.GetVal() != nullptr));


		// We might want to add a separate method for replacing uniform value instead of doing this.
		for (unsigned int i = 0; i < m_uniform_descriptors.size(); i++) {
			if (m_uniform_descriptors[i].GetName() == uniform_descriptor.GetName()) {
				if (m_uniform_descriptors[i].GetType() != uniform_descriptor.GetType()) {
					std::cerr << __FILE__ << ":" << __func__ << ": Uniform type mismatch." << std::endl;
					assert(false);
				}

				m_uniform_descriptors[i].SetVal(uniform_descriptor.GetVal(), uniform_descriptor.GetValSize());

				return;
			}
		}

		m_uniform_descriptors.push_back(uniform_descriptor);
	}

	void AddUniformBlockDescriptor(UniformBlockDescriptor uniform_block_descriptor, unsigned int binding_point)
	{
		m_uniform_block_descriptors.push_back(uniform_block_descriptor);
		m_binding_points.push_back(binding_point);
	}

private:

	std::vector<Texture2D*> m_framebuffer_textures;
	std::vector<VertexBuffer*> m_vertex_buffers;
	std::vector<VertexElementBuffer*> m_vertex_element_buffers;
	std::vector<UniformDescriptor> m_uniform_descriptors;
	std::vector<UniformBlockDescriptor> m_uniform_block_descriptors;
	std::vector<unsigned int> m_binding_points;
};