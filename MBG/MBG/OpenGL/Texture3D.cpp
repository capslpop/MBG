#include "Texture3D.hpp"

//TODO: Set texture parameter function?
//Check in with team: should we provide 4.6 functionality with another class?

//Load Texture3D with data provided
Texture3D::Texture3D(const std::filesystem::path& tex_path, size_t size_x, size_t size_y, size_t size_z, TEXTURE_TYPE type)
	: texture_(0)
	, width_(size_x)
	, height_(size_y)
	, depth_(size_z) 
{

	//Generate and bind texture
	glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_3D, texture_);

	//Set default texture parameters
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	//Create texture
	std::ifstream stream(tex_path, std::ios::binary);

	if (!stream) {
		glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH, -1,
			"Application Error: Texture3D: File failed to load.");
	}

	//Read stream into data buffer
	int file_length = std::filesystem::file_size(tex_path);
	char* data = new char[file_length];
	stream.read(data, file_length);

	glTexImage3D(GL_TEXTURE_3D, 0, std::get<0>(tex_type_map[type]), width_, height_, depth_, 0, std::get<1>(tex_type_map[type]), GL_UNSIGNED_BYTE, data);

	delete data[];
}

Texture3D::bind() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, texture_);
}

Texture3D::unbind() {
	glBindTexture(GL_TEXTURE_3D, 0);
}

Texture3D::~Texture3D() {
	glDeleteTextures(1, &texture_);
}