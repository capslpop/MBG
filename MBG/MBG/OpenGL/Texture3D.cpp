#include "Texture3D.hpp"

// Mapping is here for now
std::map<TEXTURE_TYPE, std::pair<GLint, GLenum>> tex_type_map{
	{R8, {GL_R8, GL_RED}},
	{R8_SNORM, {GL_R8_SNORM, GL_RED}},
	{R16, {GL_R16, GL_RED}},
	{R16_SNORM, {GL_R16_SNORM, GL_RED}},
	{RG8_SNORM, {GL_RG8_SNORM, GL_RG}},
	{RG16, {GL_RG16, GL_RG}},
	{RG16_SNORM, {GL_RG16_SNORM, GL_RG}},
	{R3_G3_B2, {GL_R3_G3_B2, GL_RGB}},
	{RGB4, {GL_RGB4, GL_RGB}},
	{RGB5, {GL_RGB5, GL_RGB}},
	{RGB8, {GL_RGB8, GL_RGB}},
	{RGB8_SNORM, {GL_RGB8_SNORM, GL_RGB}},
	{RGB10, {GL_RGB10, GL_RGB}},
	{RGB12, {GL_RGB12, GL_RGB}},
	{RGB16_SNORM, {GL_RGB16_SNORM, GL_RGB}},
	{RGBA2, {GL_RGBA2, GL_RGB}},
	{RGBA4, {GL_RGBA4, GL_RGB}},
	{RGB5_A1, {GL_RGB5_A1, GL_RGBA}},
	{RGBA8, {GL_RGBA8, GL_RGBA}},
	{RGBA8_SNORM, {GL_RGBA8_SNORM, GL_RGBA}},
	{RGB10_A2, {GL_RGB10_A2, GL_RGBA}},
	{RGB10_A2UI, {GL_RGB10_A2UI, GL_RGBA}},
	{RGBA12, {GL_RGBA12, GL_RGBA}},
	{RGBA16, {GL_RGBA16, GL_RGBA}},
	{SRGB8, {GL_SRGB8, GL_RGB}},
	{SRGB8_ALPHA8, {GL_SRGB8_ALPHA8, GL_RGBA}},
	{R16F, {GL_R16F, GL_RED}},
	{RG16F, {GL_RG16F, GL_RG}},
	{RGB16F, {GL_RGB16F, GL_RGB}},
	{RGBA16F, {GL_RGBA16F, GL_RGBA}},
	{R32F, {GL_R32F, GL_RED}},
	{RG32F, {GL_RG32F, GL_RG}},
	{RGB32F, {GL_RGB32F, GL_RGB}},
	{RGBA32F, {GL_RGBA32F, GL_RGBA}},
	{R11F_G11F_B10F, {GL_R11F_G11F_B10F, GL_RGB}},
	{RGB9_E5, {GL_RGB9_E5, GL_RGB}},
	{R8I, {GL_R8I, GL_RED}},
	{R8UI, {GL_R8UI, GL_RED}},
	{R16I, {GL_R16I, GL_RED}},
	{R16UI, {GL_R16UI, GL_RED}},
	{R32I, {GL_R32I, GL_RED}},
	{R32UI, {GL_R32UI, GL_RED}},
	{RG8I, {GL_RG8I, GL_RG}},
	{RG8UI, {GL_RG8UI, GL_RG}},
	{RG16I, {GL_RG16I, GL_RG}},
	{RG16UI, {GL_RG16UI, GL_RG}},
	{RG32I, {GL_RG32I, GL_RG}},
	{RG32UI, {GL_RG32UI, GL_RG}},
	{RGB8I, {GL_RGB8I, GL_RGB}},
	{RGB8UI, {GL_RGB8UI, GL_RGB}},
	{RGB16I, {GL_RGB16I, GL_RGB}},
	{RGB16UI, {GL_RGB16UI, GL_RGB}},
	{RGB32I, {GL_RGB32I, GL_RGB}},
	{RGB32UI, {GL_RGB32UI, GL_RGB}},
	{RGBA8I, {GL_RGBA8I, GL_RGBA}},
	{RGBA8UI, {GL_RGBA8UI, GL_RGBA}},
	{RGBA16I, {GL_RGBA16I, GL_RGBA}},
	{RGBA16UI, {GL_RGBA16UI, GL_RGBA}},
	{RGBA32I, {GL_RGBA32I, GL_RGBA}},
	{RGBA32UI, {GL_RGBA32UI, GL_RGBA}},
};

// Load Texture3D with data provided
Texture3D::Texture3D(const std::filesystem::path& tex_path, TextureParams params)
	: texture_(0)
	, width_(params.size_x)
	, height_(params.size_y)
	, depth_(params.size_z) 
	, type_(params.type)
	, data_type_(params.data_type)
{

	// Load glDebugMessageInsertARB
	PFNGLDEBUGMESSAGEINSERTPROC pfnGlDebugMessageInsertARB = reinterpret_cast<PFNGLDEBUGMESSAGEINSERTPROC>(glfwGetProcAddress("glDebugMessageInsertARB"));
	if (pfnGlDebugMessageInsertARB == NULL)
	{
		std::cerr << "Failed to load glDebugMessageInsertARB." << std::endl;
		return;
	}

	// Generate and bind texture
	glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_3D, texture_);

	// Set default texture parameters
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	// Create texture
	std::ifstream stream(tex_path, std::ios::binary);

	if (!stream) {
		pfnGlDebugMessageInsertARB(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0, GL_DEBUG_SEVERITY_HIGH, -1,
			"Application Error: Texture3D: File failed to load.");
	}
	else {
		pfnGlDebugMessageInsertARB(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_OTHER, 0, GL_DEBUG_SEVERITY_NOTIFICATION, -1,
			"Texture3D: Texture File loaded successfully");
	}

	// Read stream into data buffer
	uintmax_t file_length = std::filesystem::file_size(tex_path);

	std::cout << file_length << std::endl;

	char* data = new char[file_length];
	stream.read(data, file_length);

	glTexImage3D(GL_TEXTURE_3D, 0, std::get<0>(tex_type_map[type_]), width_, height_, depth_, 0, std::get<1>(tex_type_map[type_]), data_type_, data);
	glGenerateMipmap(GL_TEXTURE_3D);

	pfnGlDebugMessageInsertARB(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_OTHER, 0, GL_DEBUG_SEVERITY_NOTIFICATION, -1,
		"Texture3D: Texture loaded successfully");

	delete[] data;
}

// Load Texture3D with no data
Texture3D::Texture3D(TextureParams params)
	: texture_(0)
	, width_(params.size_x)
	, height_(params.size_y)
	, depth_(params.size_z)
	, type_(params.type)
	, data_type_(params.data_type)
{

	// Load glDebugMessageInsertARB
	PFNGLDEBUGMESSAGEINSERTPROC pfnGlDebugMessageInsertARB = reinterpret_cast<PFNGLDEBUGMESSAGEINSERTPROC>(glfwGetProcAddress("glDebugMessageInsertARB"));
	if (pfnGlDebugMessageInsertARB == NULL)
	{
		std::cerr << "Failed to load glDebugMessageInsertARB." << std::endl;
		return;
	}

	// Generate and bind texture
	glGenTextures(1, &texture_);
	glBindTexture(GL_TEXTURE_3D, texture_);

	// Set default texture parameters
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);


	glTexImage3D(GL_TEXTURE_3D, 0, std::get<0>(tex_type_map[type_]), width_, height_, depth_, 0, std::get<1>(tex_type_map[type_]), data_type_, nullptr);

	pfnGlDebugMessageInsertARB(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_OTHER, 0, GL_DEBUG_SEVERITY_NOTIFICATION, -1,
		"Texture3D: Texture loaded successfully");

}

// Set texture parameters

void Texture3D::bind(GLint slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_3D, texture_);
}

void Texture3D::unbind() {
	glBindTexture(GL_TEXTURE_3D, 0);
}

void Texture3D::set_param(GLenum pname, GLfloat param) {
	glTexParameterf(GL_TEXTURE_3D, pname, param);
}

void Texture3D::set_param(GLenum pname, GLint param) {
	glTexParameteri(GL_TEXTURE_3D, pname, param);
}

void Texture3D::set_param(GLenum pname, const GLfloat* params) {
	glTexParameterfv(GL_TEXTURE_3D, pname, params);
}

void Texture3D::set_param(GLenum pname, const GLint* params) {
	glTexParameteriv(GL_TEXTURE_3D, pname, params);
}

void Texture3D::set_param(GLenum pname, const GLuint* params) {
	glTexParameterIuiv(GL_TEXTURE_3D, pname, params);
}

Texture3D::~Texture3D() {
	glDeleteTextures(1, &texture_);
}