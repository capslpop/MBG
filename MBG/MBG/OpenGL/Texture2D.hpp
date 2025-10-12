#pragma once

#include "Types.hpp"

class Texture2D {
public:
	Texture2D(size_t size_x, size_t size_y, TEXTURE_TYPE type, void* data = nullptr) {

		// Make sure to double check the byte alignment when sending data smaller than 4 bytes per x pixel length

	}

	~Texture2D() {

	}

private:

};