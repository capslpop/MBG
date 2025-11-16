#pragma once

#include <glad/glad.h>
#include <map>
#include <utility>

struct TextureParams {
	// Actual texture parameters (size, type, etc.)
	GLsizei size_x;
	GLsizei size_y;
	GLsizei size_z;
	TEXTURE_TYPE type = TEXTURE_TYPE::RGBA16F;
	GLenum data_type = GL_UNSIGNED_INT;

	// Texture parameters for glTextureParameter
	GLint min_filter = GL_LINEAR;
	GLint mag_filter = GL_LINEAR;
	GLint wrap_s = GL_CLAMP_TO_BORDER;
	GLint wrap_t = GL_CLAMP_TO_BORDER;
	GLint wrap_r = GL_CLAMP_TO_BORDER;

	/* 
	* Texture parameters that might get implemented
	* GLint min_lod;
	* GLint max_lod;
	* GLint max_level;
	* GLint swizzle_rgba;
	* GLint depth_stencil_tex_mode;
	* GLint tex_base_lvl;
	* GLfloat tex_border_color;
	* GLint tex_compare_func;
	* GLint tex_compare_mode;
	* GLint tex_lod_bias;
	*/

};