#pragma once

#include "Types.hpp"

class Texture2D {
public:
	struct Texture2DParameters {
		GLenum wrap_s;
		GLenum wrap_t;
		GLenum min_filter;
		GLenum mag_filter;
	};

	struct Texture2DImage {
		GLsizei size_x;
		GLsizei size_y;
		TEXTURE_TYPE type;
		void* data = nullptr;
	};

	struct Texture2DArgs {
		Texture2DImage image;
		Texture2DParameters params;
		bool mipmap_en;
	};

	Texture2D(Texture2DArgs input): type_(tex_type_map[input.image.type].second) {
		glGenTextures(1, &ID);
		setParameters(input.params);

		if (input.image.data) glTexImage2D(type_, 0, tex_type_map[input.image.type].first, 
										   input.image.size_x, input.image.size_x, 0, 
										   tex_type_map[input.image.type].first, 
										   GL_UNSIGNED_BYTE, input.image.data);
		if (input.mipmap_en) glGenerateMipmap(type_);
	}

	void setParameters(Texture2DParameters input) {
		glBindTexture(type_, ID);

		glTexParameteri(type_, GL_TEXTURE_WRAP_S, input.wrap_s);
		glTexParameteri(type_, GL_TEXTURE_WRAP_T, input.wrap_t);
		glTexParameteri(type_, GL_TEXTURE_MIN_FILTER, input.min_filter);
		glTexParameteri(type_, GL_TEXTURE_MAG_FILTER, input.mag_filter);
	}

	void setImage(Texture2DImage input) {
		glBindTexture(type_, ID);

		if (input.data) glTexImage2D(type_, 0, tex_type_map[input.type].first, 
									 input.size_x, input.size_y, 0, 
									 tex_type_map[input.type].first, 
									 GL_UNSIGNED_BYTE, input.data);
	}

	void bind(int slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(type_, ID);
	}

	void unbind() {
		glBindTexture(type_, 0);
	}

	~Texture2D() {
		glDeleteTextures(1, &ID);
	}



private:
	unsigned int ID;
	GLenum type_;
	GLint color_type_;
};