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

	Texture2D(Texture2DArgs input) {
		glGenTextures(1, &ID);
		setParameters(input.params);

		if (input.image.data) glTexImage2D(GL_TEXTURE_2D, 0, tex_type_map[input.image.type].second,
			input.image.size_x, input.image.size_x, 0,
			tex_type_map[input.image.type].second,
			GL_UNSIGNED_BYTE, input.image.data);
		if (input.mipmap_en) glGenerateMipmap(GL_TEXTURE_2D);
	}

	void setParameters(Texture2DParameters input) {
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, input.wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, input.wrap_t);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, input.min_filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, input.mag_filter);
	}

	void setImage(Texture2DImage input) {
		glBindTexture(GL_TEXTURE_2D, ID);

		if (input.data) glTexImage2D(GL_TEXTURE_2D, 0, tex_type_map[input.type].second,
			input.size_x, input.size_y, 0,
			tex_type_map[input.type].second,
			GL_UNSIGNED_BYTE, input.data);
	}

	void bind(int slot) {
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, ID);
	}

	void unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	~Texture2D() {
		glDeleteTextures(1, &ID);
	}



private:
	unsigned int ID;
};