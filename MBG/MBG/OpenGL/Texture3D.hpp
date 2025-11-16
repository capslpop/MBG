#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "Types.hpp"
#include "Structs.hpp"

class Texture3D {
	public:
		Texture3D(const std::filesystem::path& tex_path, TextureParams params);
		Texture3D(TextureParams params);

		void bind(GLint slot);
		void unbind();

		~Texture3D();

	private:
		GLuint texture_;
		TEXTURE_TYPE type_;
		GLsizei width_;
		GLsizei height_;
		GLsizei depth_;
		GLenum data_type_;
};