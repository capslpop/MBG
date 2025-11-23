#pragma once 

#include <vector>
#include <cassert>

#include "../CommandBuffer.hpp"
#include "../FrameGraph.hpp"
#include "../Info.hpp"

#include "VertexBuffer.hpp"
#include "ElementBuffer.hpp"
#include "InstancedVertexBuffer.hpp"
#include "InstancedElementBuffer.hpp"
#include "TextureBuffer.hpp"
#include "Texture2DBuffer.hpp"
#include "Texture3DBuffer.hpp"
#include "TextureCubeMapBuffer.hpp"
#include "Texture2DArrayBuffer.hpp"
#include "UniformBuffer.hpp"

namespace MBG {

class DescriptorSetBuffer {
public:
	DescriptorSetBuffer(const DescriptorSetBufferParams& params) {
		// First we generate all bindings for in buffers
		addInDescriptors(params.descriptors, params.count);

		// Second we generate bind for trasform feedback
		if (is_transform_feedback_) {
			glGenTransformFeedbacks(1, &transform_feedback_id_);
			addTransformFeedBackDescriptor(params.descriptors, params.count);
		}

		// Third we generate binding for frame buffers
		if (is_frame_buffer_) {
			glGenFramebuffers(1, &frame_buffer_id_);
			addFrameBufferDescriptor(params.descriptors, params.count);
		}
	}

	~DescriptorSetBuffer() {
		if (is_transform_feedback_) {
			glDeleteTransformFeedbacks(1, &transform_feedback_id_);
		}
		if (is_frame_buffer_) {
			glDeleteFramebuffers(1, &frame_buffer_id_);
		}
	}

protected:
	void bind(CommandBuffer& command_buffer) {
		for (const auto& b : binds_) {


			// 
			//
			// TODO!!!!!!!!!!!!!!!!!!! We need to select which part of the buffer to use dynamically
			//
			//


			switch (b.type) {

			case BINDIND_TYPE::TEXTURE_BUFFER:
				command_buffer.activeTexture(GL_TEXTURE0 + b.slot);
				command_buffer.bindTexture(GL_TEXTURE_BUFFER, b.id);
				break;

			case BINDIND_TYPE::TEXTURE_2D:
				command_buffer.activeTexture(GL_TEXTURE0 + b.slot);
				command_buffer.bindTexture(GL_TEXTURE_2D, b.id);
				break;

			case BINDIND_TYPE::TEXTURE_3D:
				command_buffer.activeTexture(GL_TEXTURE0 + b.slot);
				command_buffer.bindTexture(GL_TEXTURE_3D, b.id);
				break;

			case BINDIND_TYPE::TEXTURE_CUBE_MAP:
				command_buffer.activeTexture(GL_TEXTURE0 + b.slot);
				command_buffer.bindTexture(GL_TEXTURE_CUBE_MAP, b.id);
				break;

			case BINDIND_TYPE::TEXTURE_ARRAY:
				command_buffer.activeTexture(GL_TEXTURE0 + b.slot);
				command_buffer.bindTexture(GL_TEXTURE_2D_ARRAY, b.id);
				break;

			case BINDIND_TYPE::UNIFORM:
				command_buffer.bindBufferBase(GL_UNIFORM_BUFFER, b.slot, b.id);
				break;

			case BINDIND_TYPE::FRAME_BUFFER:
				command_buffer.bindFramebuffer(GL_FRAMEBUFFER, b.id);
				break;

			case BINDIND_TYPE::TRANSFORM_FEEDBACK:
				command_buffer.bindTransformFeedback(GL_TRANSFORM_FEEDBACK, b.id);
				break;

			default:
				assert(false);
				break;
			}
		}
	}

	void draw(CommandBuffer& command_buffer, GLenum render_type, GLsizei draw_count) {
		for (const auto& i : vaos_)
		{
			command_buffer.bindVertexArray(i.id);

			switch (i.type) {
			case BINDIND_TYPE::VERTEX_ARRAY:

				command_buffer.drawArraysInstanced(
					render_type,
					i.start,
					i.size,
					draw_count
				);

				break;

			case BINDIND_TYPE::VERTEX_ELEMENT_ARRAY:

				command_buffer.drawElementsInstanced(
					render_type,
					i.size,
					GLenum(i.element_type),
					(void*)size_t(i.start * i.byte_size),
					draw_count
				);

				break;

			default:
				break;
			}
		}
	}
	
	uvec2 frame_buffer_size_ = uvec2(0, 0);

	friend class FrameGraph;

private:
	enum class BINDIND_TYPE {
		VERTEX_ARRAY,
		VERTEX_ELEMENT_ARRAY,
		TEXTURE_BUFFER,
		TEXTURE_2D,
		TEXTURE_3D,
		TEXTURE_CUBE_MAP,
		TEXTURE_ARRAY,
		UNIFORM,
		FRAME_BUFFER,
		TRANSFORM_FEEDBACK,
	};

	struct Binding {
		BINDIND_TYPE type;
		uint id;
		uint slot;
	};

	struct VaoBinding {
		BINDIND_TYPE type;
		uint id;
		ELEMENT_ATTRUBUTE element_type;	// used by element buffer for indicies
		uint start;				// offset from the begging
		uint byte_size;			// byte size of the indices
		uint size;				// always number of vertices
	};

	std::vector<Binding> binds_;
	std::vector<VaoBinding> vaos_;
	
	bool is_transform_feedback_ = false;
	GLuint transform_feedback_id_ = 0u;

	bool is_frame_buffer_ = false;
	GLuint frame_buffer_id_ = 0u;

	void addInDescriptors(const Descriptor* descriptor_set, const uint count) {
		uint texture_slots = 0;
		uint uniform_slots = 0;

		for (unsigned i = 0; i < count; i++) {
			const Descriptor& d = descriptor_set[i];

			switch (d.type) {


			// TODO:          HERE WE NEED TO ADD THE NULL STUFF

			case DESCRIPTOR_TYPE::VERTEX_BUFFER_IN:{
				VertexBuffer* e = (VertexBuffer*)d.data;

				if (d.view_size == 0) {
					vaos_.emplace_back(BINDIND_TYPE::VERTEX_ARRAY, e->vertex_array_id_, ELEMENT_ATTRUBUTE::BYTE, 0, 0, e->size_);
				}
				else {
					vaos_.emplace_back(BINDIND_TYPE::VERTEX_ARRAY, e->vertex_array_id_, ELEMENT_ATTRUBUTE::BYTE, d.view_start, 0, d.view_size);
				}

				break;
			}

			case DESCRIPTOR_TYPE::ELEMENT_BUFFER_IN: {
				ElementBuffer* e = (ElementBuffer*)d.data;
				vaos_.emplace_back(BINDIND_TYPE::VERTEX_ELEMENT_ARRAY, e->vertex_array_id_, e->element_attrib_, d.view_start, e->element_byte_size_, d.view_size);
				break;
			}

			case DESCRIPTOR_TYPE::INSTANCED_VERTEX_BUFFER_IN: {
				InstancedVertexBuffer* e = (InstancedVertexBuffer*)d.data;
				vaos_.emplace_back(BINDIND_TYPE::VERTEX_ARRAY, e->vertex_array_id_, ELEMENT_ATTRUBUTE::BYTE, d.view_start, 0, d.view_size);
				break;
			}

			case DESCRIPTOR_TYPE::INSTANCED_ELEMENT_BUFFER_IN: {
				InstancedElementBuffer* e = (InstancedElementBuffer*)d.data;
				vaos_.emplace_back(BINDIND_TYPE::VERTEX_ELEMENT_ARRAY, e->vertex_array_id_, e->element_attrib_, d.view_start, e->element_byte_size_, d.view_size);
				break;
			}

			case DESCRIPTOR_TYPE::TEXTURE_BUFFER_IN: {
				TextureBuffer* e = (TextureBuffer*)d.data;
				binds_.emplace_back(BINDIND_TYPE::TEXTURE_BUFFER, e->texture_id_, texture_slots++);
				break;
			}

			case DESCRIPTOR_TYPE::TEXTURE_2D_BUFFER_IN: {
				Texture2DBuffer* e = (Texture2DBuffer*)d.data;
				binds_.emplace_back(BINDIND_TYPE::TEXTURE_2D, e->texture_id_, texture_slots++);
				break;
			}

			case DESCRIPTOR_TYPE::TEXTURE_3D_BUFFER_IN: {
				Texture3DBuffer* e = (Texture3DBuffer*)d.data;
				binds_.emplace_back(BINDIND_TYPE::TEXTURE_3D, e->texture_id_, texture_slots++);
				break;
			}
			
			case DESCRIPTOR_TYPE::TEXTURE_CUBE_MAP_BUFFER_IN: {
				TextureCubeMapBuffer* e = (TextureCubeMapBuffer*)d.data;
				binds_.emplace_back(BINDIND_TYPE::TEXTURE_CUBE_MAP, e->texture_id_, texture_slots++);
				break;
			}

			case DESCRIPTOR_TYPE::TEXTURE_ARRAY_BUFFER_IN: {
				Texture2DArrayBuffer* e = (Texture2DArrayBuffer*)d.data;
				binds_.emplace_back(BINDIND_TYPE::TEXTURE_ARRAY, e->texture_id_, texture_slots++);
				break;
			}

			case DESCRIPTOR_TYPE::UNIFORM_BUFFER: {
				UniformBuffer* e = (UniformBuffer*)d.data;
				binds_.emplace_back(BINDIND_TYPE::UNIFORM, e->uniform_id_, uniform_slots++);
				break;
			}

			case DESCRIPTOR_TYPE::VERTEX_BUFFER_OUT:
			case DESCRIPTOR_TYPE::ELEMENT_BUFFER_OUT:
			case DESCRIPTOR_TYPE::INSTANCED_VERTEX_BUFFER_OUT:
			case DESCRIPTOR_TYPE::INSTANCED_ELEMENT_BUFFER_OUT:
				is_transform_feedback_ = true;
				break;

			case DESCRIPTOR_TYPE::TEXTURE_BUFFER_OUT:
			case DESCRIPTOR_TYPE::TEXTURE_2D_BUFFER_OUT:
			case DESCRIPTOR_TYPE::TEXTURE_3D_BUFFER_OUT:
			case DESCRIPTOR_TYPE::TEXTURE_CUBE_MAP_BUFFER_OUT:
				is_frame_buffer_ = true;
				break;

			default:
				break;
			}
		}
	}

	void addTransformFeedBackDescriptor(const Descriptor* descriptor_set, const uint count) {
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transform_feedback_id_);

		GLuint binding_index = 0;
		for (unsigned i = 0; i < count; i++) {
			const Descriptor& d = descriptor_set[i];

			switch (d.type) {
			case DESCRIPTOR_TYPE::VERTEX_BUFFER_OUT: {
				VertexBuffer* vb = (VertexBuffer*)d.data;

				if (d.view_size == 0) { // bind whole buffer
					glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, binding_index++, vb->vertex_buffer_id_);
				}
				else { // bind only a range
					uint bytes_per_element = vb->struct_size_;
					glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, binding_index++,
						vb->vertex_buffer_id_,
						d.view_start * bytes_per_element,
						d.view_size * bytes_per_element);
				}
				break;
			}

			case DESCRIPTOR_TYPE::ELEMENT_BUFFER_OUT: {
				ElementBuffer* eb = (ElementBuffer*)d.data;

				if (d.view_size == 0) {
					glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, binding_index++, eb->vertex_buffer_id_);
				}
				else {
					uint bytes_per_element = eb->struct_size_;
					glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, binding_index++,
						eb->vertex_buffer_id_,
						d.view_start * bytes_per_element,
						d.view_size * bytes_per_element);
				}
				break;
			}

			case DESCRIPTOR_TYPE::INSTANCED_VERTEX_BUFFER_OUT: {
				InstancedVertexBuffer* ivb = (InstancedVertexBuffer*)d.data;

				if (d.view_size == 0) {
					glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, binding_index++, ivb->vertex_buffer_id_);
				}
				else {
					uint bytes_per_element = ivb->struct_size_;
					glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, binding_index++,
						ivb->vertex_buffer_id_,
						d.view_start * bytes_per_element,
						d.view_size * bytes_per_element);
				}
				break;
			}

			case DESCRIPTOR_TYPE::INSTANCED_ELEMENT_BUFFER_OUT: {
				InstancedElementBuffer* ieb = (InstancedElementBuffer*)d.data;

				if (d.view_size == 0) {
					glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, binding_index++, ieb->vertex_buffer_id_);
				}
				else {
					uint bytes_per_element = ieb->struct_size_;
					glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, binding_index++,
						ieb->vertex_buffer_id_,
						d.view_start * bytes_per_element,
						d.view_size * bytes_per_element);
				}
				break;
			}

			default:
				break;
			}
		}

		binds_.emplace_back(BINDIND_TYPE::TRANSFORM_FEEDBACK, transform_feedback_id_, 0);
	}
	
	void addFrameBufferDescriptor(const Descriptor* descriptor_set, const uint count) {
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_id_);

		GLuint color_attachment_index = 0;

		for (unsigned i = 0; i < count; i++) {
			const Descriptor& d = descriptor_set[i];

			switch (d.type) {

			case DESCRIPTOR_TYPE::TEXTURE_BUFFER_OUT: {
				TextureBuffer* tb = (TextureBuffer*)d.data;

				if (d.view_size == 0) {
					// Bind entire buffer
					glFramebufferTexture(GL_FRAMEBUFFER,
						GL_COLOR_ATTACHMENT0 + color_attachment_index++,
						tb->texture_id_,
						0);
				}
				else {
					// Bind multiple mip levels
					for (unsigned mip = d.view_start; mip < d.view_start + d.view_size; mip++) {
						glFramebufferTexture(GL_FRAMEBUFFER,
							GL_COLOR_ATTACHMENT0 + color_attachment_index++,
							tb->texture_id_,
							mip);
					}
				}
				break;
			}

			case DESCRIPTOR_TYPE::TEXTURE_2D_BUFFER_OUT: {
				Texture2DBuffer* tex2D = (Texture2DBuffer*)d.data;

				if (d.view_size == 0) {
					glFramebufferTexture2D(GL_FRAMEBUFFER,
						GL_COLOR_ATTACHMENT0 + color_attachment_index++,
						GL_TEXTURE_2D,
						tex2D->texture_id_,
						0);
				}
				else {
					for (unsigned mip = d.view_start; mip < d.view_start + d.view_size; mip++) {
						glFramebufferTexture(GL_FRAMEBUFFER,
							GL_COLOR_ATTACHMENT0 + color_attachment_index++,
							tex2D->texture_id_,
							mip);
					}
				}


				break;
			}

			case DESCRIPTOR_TYPE::TEXTURE_3D_BUFFER_OUT: {
				Texture3DBuffer* tex3D = (Texture3DBuffer*)d.data;

				uint start = d.view_start;
				uint size = d.view_size;
				if (d.view_size == 0) { // Bind full texture
					size = tex3D->getSize().z;
					start = 0; // set to origin
				}

				for (unsigned slice = 0; slice < d.view_size; slice++) {
					glFramebufferTextureLayer(GL_FRAMEBUFFER,
						GL_COLOR_ATTACHMENT0 + color_attachment_index++,
						tex3D->texture_id_,
						0,                 
						d.view_start + slice);
				}
				break;
			}

			case DESCRIPTOR_TYPE::TEXTURE_CUBE_MAP_BUFFER_OUT: {
				TextureCubeMapBuffer* cubeTex = (TextureCubeMapBuffer*)d.data;

				uint start = d.view_start;
				uint size = d.view_size;
				if (d.view_size == 0) { // Bind full texture
					size = 6; // add 6 faces
					start = 0;
				}

				for (unsigned face = start; face < start + size; face++) {
					GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + face;
					glFramebufferTexture2D(GL_FRAMEBUFFER,
						GL_COLOR_ATTACHMENT0 + color_attachment_index++,
						target,
						cubeTex->texture_id_,
						0);
				}
				break;
			}

			case DESCRIPTOR_TYPE::TEXTURE_ARRAY_BUFFER_OUT: {
				Texture2DArrayBuffer* texArray = (Texture2DArrayBuffer*)d.data;

				uint start = d.view_start;
				uint size = d.view_size;
				if (d.view_size == 0) { // Bind full texture
					size = texArray->getLayerSize();
					start = 0;
				}

				for (unsigned layer = 0; layer < d.view_size; layer++) {
					glFramebufferTextureLayer(GL_FRAMEBUFFER,
						GL_COLOR_ATTACHMENT0 + color_attachment_index++,
						texArray->texture_id_,
						0,
						d.view_start + layer);
				}
				break;
			}

			default:
				break;
			}
		}

		binds_.emplace_back(BINDIND_TYPE::FRAME_BUFFER, frame_buffer_id_, 0);
	}

};

}