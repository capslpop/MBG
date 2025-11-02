#pragma once

#include "Types.hpp"

class Texture2D {
public:
	struct Texture2DArgs {
		GLsizei size_x;
		GLsizei size_y;
		TEXTURE_TYPE type;
		void* data = nullptr;
		GLenum wrap_s;
		GLenum wrap_t;
		GLenum min_filter;
		GLenum mag_filter;
		bool mipmap_en;
	};

	Texture2D(Texture2DArgs input) {
		type_ = tex_type_map[input.type].second;
		glGenTextures(1, &ID);
		glBindTexture(type_, ID);

		glTexParameteri(type_, GL_TEXTURE_WRAP_S, input.wrap_s);
		glTexParameteri(type_, GL_TEXTURE_WRAP_T, input.wrap_t);
		glTexParameteri(type_, GL_TEXTURE_MIN_FILTER, input.min_filter);
		glTexParameteri(type_, GL_TEXTURE_MAG_FILTER, input.mag_filter);

		glTexImage2D(type_, 0, GL_RGB, input.size_x, input.size_y, 0, GL_RGB, GL_UNSIGNED_BYTE, input.data);

		if (input.mipmap_en) glGenerateMipmap(type_);
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
};