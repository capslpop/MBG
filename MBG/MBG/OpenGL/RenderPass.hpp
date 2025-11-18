#pragma once
#include <fstream>
#include <iostream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#include "MBG.hpp"

class RenderPass
{
public:
	RenderPass(std::string vertexPath, std::string fragmentPath);

	~RenderPass();

	// Delete copy constructors
	RenderPass(const RenderPass& other) = delete;
	RenderPass& operator=(const RenderPass& other) = delete;

	/*
		Gets the shader program
	*/
	inline GLuint getProgram();


	/*
		This does the bulk of the operations in this class.

		For each shader we need to
		1.) Add the include files that are referenced. We should use the shader files parent directory for the reference point.
		2.) Add the correct memory object definition and name
			for example if we have a 2D texture with name "light_info" we should include it like this sampler2D light_info;

		Finally we need to link all of the objects into one object.
	*/
	void link();

protected:
	std::string vertexFile;
	std::string fragmentFile;
	GLuint shaderProgram;
};