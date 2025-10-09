#pragma once

#include <cassert>
#include <cstring>
#include <glad/glad.h>
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include <GL/glext.h>
#include <iostream>

class Window
{
public:
	Window(int width, int height);

	bool isClosed() const;
	void pullAndSwapBuffers() const;

	inline int getWidth() const { return width_; };
	inline int getHeight() const { return height_; };

	~Window();

	GLFWwindow* window;

private:
	static void framebuffer_size_callback_static(GLFWwindow* window, int width, int height);
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	void startGLFWDebug();
	void startOpenGLDebug();

	void checkGLError(const char* msg = "");

	static void glDebugOutput(GLenum source,
		GLenum type,
		unsigned int id,
		GLenum severity,
		GLsizei length,
		const char* message,
		const void* userParam);

	int width_;
	int height_;
};