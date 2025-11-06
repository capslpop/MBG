#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>
#include "Texture2D.hpp"
#include "VertexBuffer.hpp"

class UniformDescriptorSet {
	public:
		UniformDescriptorSet(std::string name, std::string type, uint8_t* val) {
			assert(!name.empty() && type == "3f");
			uniform_name = name;
			uniform_type = type;
			uniform_val = val;
		}
		inline void SetName(const std::string name) {
			assert(!name.empty());
			uniform_name = name;
		}

		inline void SetType(const std::string type) {
			assert(type == "3f"); 
			uniform_type = type;
		}

		inline void SetVal(const uint8_t* val) {
			delete uniform_val;
			uniform_val = new uint8_t(*val);// could use smart pointers, but I dont have experience with them, maybe someone else.
		}

		inline std::string GetName() { return uniform_name; }

		inline std::string GetType() { return uniform_type; }

		inline const uint8_t* GetVal() { return uniform_val; }
	private:
		std::string uniform_name;
		std::string uniform_type;
		uint8_t* uniform_val = nullptr;
};

class DescriptorSet {
	public:
		DescriptorSet() : m_framebuffer_texture(nullptr), m_vertex_buffer(nullptr) {
		}

		DescriptorSet(Texture2D* p_framebuffer_texture, VertexBuffer* p_vertex_buffer) : m_framebuffer_texture(p_framebuffer_texture), m_vertex_buffer(p_vertex_buffer) {
		}

		inline void SetFramebufferTexture(Texture2D* p_framebuffer_texture) {
			m_framebuffer_texture = p_framebuffer_texture;
		}

		inline void SetVertexBuffer(VertexBuffer* p_vertex_buffer) {
			m_vertex_buffer = p_vertex_buffer;
		}

		void AddUniformDescriptor(UniformDescriptorSet uniform_descriptor_set) {
			assert(!(uniform_descriptor_set.GetName().empty() || uniform_descriptor_set.GetType().empty()) && (uniform_descriptor_set.GetVal() != nullptr));

			
			// We might want to add a separate method for replacing uniform value instead of doing this.
			for (unsigned int i = 0; i < m_uniform_descriptors.size(); i++) {
				if (m_uniform_descriptors[i].GetName() == uniform_descriptor_set.GetName()) {
					if (m_uniform_descriptors[i].GetType() != uniform_descriptor_set.GetType()) {
						std::cerr << __FILE__ << ":" << __func__ << ": Uniform type mismatch." << std::endl;
						assert(false);
					}

					m_uniform_descriptors[i].SetVal(uniform_descriptor_set.GetVal());

					return;
				}
			}
			
			m_uniform_descriptors.push_back(uniform_descriptor_set);
		}

		inline Texture2D* GetFramebufferTexture() {
			return m_framebuffer_texture;
		}

		inline VertexBuffer* GetVertexBuffer() {
			return m_vertex_buffer;
		}

		inline bool HasFramebufferTexture() const {
			return m_framebuffer_texture != nullptr;
		}

		inline bool HasVertexBuffer() const {
			return m_vertex_buffer != nullptr;
		}

		inline bool HasUniformDescriptors() const {
			return m_uniform_descriptors.size() != 0;
		}

		inline const std::vector<UniformDescriptorSet>& GetUniformDescriptors() const {
			return m_uniform_descriptors;
		}

	private:
		Texture2D* m_framebuffer_texture;
		VertexBuffer* m_vertex_buffer;
		std::vector<UniformDescriptorSet> m_uniform_descriptors;
};