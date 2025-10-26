#pragma once
#include <fstream>
#include <iostream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Texture2D.hpp"
#include "VertexBuffer.hpp"
#include "DoubleTexture2D.hpp"
//#include "MBG.hpp"

class RenderPass {
public:
	RenderPass(std::string vertexPath, std::string fragmentPath, Texture2D frameBufferTexture, DoubleTexture2D doubleBufferTexture, VertexBuffer VB);
	~RenderPass();

	/*
		Stores the shader's file path
	*/
	void addVertexShader(std::string shader_file);

	/*
		Stores the shader's file path
	*/
	void addFragmentShader(std::string shader_file);

	/*
		Stores the texture that will be writen to
	*/
	void addFrameBuffer(Texture2D texture);

	/*
		Stores the double buffer textures frame
	*/
	void addDoubleBuffer(DoubleTexture2D texture);

	/*
		Stores the memory's c++ object pointer
	*/
	void bindMemory(VertexBuffer memory); // I'm assuming 'Vertices' became VertexBuffer at some point, so I'm changing this.

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
	Texture2D frameBuffer;
	DoubleTexture2D doubleBuffer;
	VertexBuffer memory;

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;
};