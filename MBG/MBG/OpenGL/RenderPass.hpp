#pragma once

class RenderPass {
public:
	RenderPass();
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
	void bindMemory(Vertices memory);

	/*
		This does the bulk of the operations in this class.

		For each shader we need to
		1.) Add the include files that are refrenced. We should use the shader files parent directory for the refrence point.
		2.) Add the correct memory object definition and name
			for example if we have a 2D texture with name "light_info" we should include it like this smapler2D light_info;
		
		Finally we need to link all of the objects into one object.
	*/
	link();

private:


};