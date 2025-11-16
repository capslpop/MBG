#pragma once

#include <glad/glad.h>
#include <map>
#include <utility>

struct TextureParams {
	GLsizei size_x;
	GLsizei size_y;
	GLsizei size_z;
	TEXTURE_TYPE type = TEXTURE_TYPE::RGBA16F;
	GLenum data_type = GL_UNSIGNED_INT;
};