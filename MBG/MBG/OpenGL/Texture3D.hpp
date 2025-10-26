#pragma once

#include <glad/glad.h>
#include <filesystem>
#include <fstream>
#include "Types.hpp"

class Texture3D {
	public:
		Texture3D(const std::filesystem::path& tex_path, size_t size_x, size_t size_y, size_t size_z, TEXTURE_TYPE type);

		bind();
		unbind();

		~Texture3D();

	private:
		GLuint texture_;
		GLuint type_;
		GLsizei width_;
		GLsizei height_;
		GLsizei depth_;
};