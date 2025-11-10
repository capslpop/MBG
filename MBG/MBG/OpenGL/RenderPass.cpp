#include "RenderPass.hpp"

RenderPass::RenderPass(std::string vertexPath, std::string fragmentPathB) : vertexFile(vertexPath), fragmentFile(fragmentPath)
{
	link();
}

RenderPass::~RenderPass()
{
	glDeleteProgram(shaderProgram);
}

inline GLuint RenderPass::getProgram()
{
	return shaderProgram;
}

void RenderPass::link()
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	try { //open and read shaders
		vShaderFile.open(vertexFile);
		fShaderFile.open(fragmentFile);

		std::stringstream vertexStream, fragmentStream;
		vertexStream << vShaderFile.rdbuf();
		fragmentStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "Error reading shaders: " << e.what() << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// build/complile shaders
	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	//check for vertex errors
	int success;
	char log[512];
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, log);
		std::cout << "Error compiling vertex shader:\n" << log << std::endl;
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	//check for fragment errors
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, log);
		std::cout << "Error compiling fragment shader:\n" << log << std::endl;
	}

	//link shaders
	GLuint newProgram = glCreateProgram();
	if (shaderProgram == 0) {
		std::cout << "Error creating shader program" << std::endl;
	}
	glAttachShader(vertex, shaderProgram);
	glAttachShader(fragment, shaderProgram);
	glLinkProgram(shaderProgram);
	//check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, log);
		std::cout << "Error linking shaders\n" << log << std::endl;
	}
	shaderProgram = newProgram;
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}