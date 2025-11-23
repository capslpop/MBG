#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
using namespace glm;

#include "../MBG/OpenGL/MBG.hpp"
using namespace MBG;

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>



//TEST_CASE("Simple Triangle Native") {
//	// ----------------- Window -----------------------------
//	/*
//	if (!glfwInit()) {
//		std::cerr << "Failed to initialize GLFW\n";
//		return;
//	}
//
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	GLFWwindow* window = glfwCreateWindow(800, 600, "Triangle", nullptr, nullptr);
//	if (!window) {
//		std::cerr << "Failed to create GLFW window\n";
//		glfwTerminate();
//		return;
//	}
//
//	glfwMakeContextCurrent(window);
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//		std::cerr << "Failed to initialize GLAD\n";
//		return;
//	}
//	*/
//	Window window(800, 600, "Triangle");
//
//	// ----------------- Vertex Buffer -----------------------------
//	/*
//	struct Vertex {
//		vec3 position;
//	};
//
//	std::vector<Vertex> vertices = {
//		{vec3(-1.0f, -1.0f, 0.0f)},
//		{vec3(1.0f, -1.0f, 0.0f)},
//		{vec3(0.0f,  1.0f, 0.0f)}
//	};
//
//	GLuint vao, vbo;
//	glGenVertexArrays(1, &vao);
//	glGenBuffers(1, &vbo);
//
//	glBindVertexArray(vao);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
//
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//	*/
//
//	struct Vertex {
//		vec3 position;
//	};
//
//	Attributes vertex_attributes[] = {
//		{ATTRIBUTE::FLOAT, 3}	// postion
//	};
//
//	std::vector<Vertex> vertex_data = {
//		{vec3(-1.0, -1.0,  0.0)},
//		{vec3(1.0, -1.0,  0.0)},
//		{vec3(1.0,  1.0,  0.0)}
//	};
//
//	VertexBuffer vertex_buffer({
//		.attributes = vertex_attributes,
//		.attributes_count = sizeof(vertex_attributes) / sizeof(Attributes),
//		.data = vertex_data.data(),
//		.count = (uint)vertex_data.size(),
//		});
//
//	// ----------------- Simple Shader -----------------------------
//	/*
//	const char* vertex_shader_src = R"(
//        #version 410 core
//        layout(location = 0) in vec3 position;
//        void main() {
//            gl_Position = vec4(position, 1.0);
//        }
//    )";
//
//	const char* fragment_shader_src = R"(
//        #version 410 core
//        out vec4 frag_color;
//        void main() {
//            frag_color = vec4(1.0, 1.0, 1.0, 1.0);
//        }
//    )";
//
//	auto compileShader = [](GLenum type, const char* src) -> GLuint {
//		GLuint shader = glCreateShader(type);
//		glShaderSource(shader, 1, &src, nullptr);
//		glCompileShader(shader);
//		GLint success;
//		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//		if (!success) {
//			char info[512];
//			glGetShaderInfoLog(shader, 512, nullptr, info);
//			std::cerr << "Shader compile error: " << info << "\n";
//		}
//		return shader;
//		};
//
//	GLuint vs = compileShader(GL_VERTEX_SHADER, vertex_shader_src);
//	GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragment_shader_src);
//
//	GLuint program = glCreateProgram();
//	glAttachShader(program, vs);
//	glAttachShader(program, fs);
//	glLinkProgram(program);
//	glUseProgram(program);
//
//	glDeleteShader(vs);
//	glDeleteShader(fs);
//	*/
//	RenderPass render_pass_main("Shaders/simple_triangle.glsl");
//
//	// ----------------- Render Loop -----------------------------
//	while (!window.isClosed()) {
//		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//		//glClear(GL_COLOR_BUFFER_BIT);
//
//		glUseProgram(render_pass_main.shader_program_);
//
//		glBindVertexArray(vertex_buffer.vertex_array_id_);
//		glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 1);
//
//		glfwSwapBuffers(window.getWindow());
//		glfwPollEvents();
//	}
//}

TEST_CASE("Simple Triangle") {
	// ----------------- Window -----------------------------
	Window window(800, 600, "Triangle");



	// ----------------- Vertex Buffer -----------------------------
	struct Vertex {
		vec3 position;
		vec3 color;
	};

	Attributes vertex_attributes[] = {
		{ATTRIBUTE::FLOAT, 3},	// postion
		{ATTRIBUTE::FLOAT, 3},	// color
	};

	std::vector<Vertex> vertex_data = {
		{vec3(-0.5, -0.5,  0.0), vec3(1.0, 0.0, 0.0)},
		{vec3( 0.5, -0.5,  0.0), vec3(0.0, 1.0, 0.0)},
		{vec3( 0.0,  0.5,  0.0), vec3(0.0, 0.0, 1.0)}
	};

	VertexBuffer vertex_buffer({
		.attributes = vertex_attributes,
		.attributes_count = sizeof(vertex_attributes) / sizeof(Attributes),
		.data = vertex_data.data(),
		.count = (uint)vertex_data.size(),
	});



	// ----------------- Render Pass -----------------------------
	RenderPass render_pass_main("Shaders/simple_triangle.glsl");



	// ----------------- Descriptor Set -----------------------------
	Descriptor descriptors[] = {
		{DESCRIPTOR_TYPE::VERTEX_BUFFER_IN, (void*)&vertex_buffer, 0, 0},
	};

	DescriptorSetBuffer descriptor_set({
		.descriptors = descriptors,
		.count = sizeof(descriptors) / sizeof(Descriptor),
	});



	// ----------------- Construct Frame Graph -----------------------------
	FrameGraph graph(window);

	// Render the geometry
	graph.addNode({
		.render_pass = &render_pass_main,
		.render_states = {},
		.descriptor_set = &descriptor_set,
	});

	graph.addNodeDisplay(); // Finally we display the frame

	graph.build();

	while (!window.isClosed()) {
		graph.run();
	}
}

/*
TEST_CASE("Simple Triangle") {
	// ----------------- Window -----------------------------
	Window window(800, 600, "Triangle");



	// ----------------- Vertex Buffer -----------------------------
	struct Vertex {
		vec3 position;
		vec2 uv;
	};

	Attributes vertex_attributes[] = {
		{ATTRIBUTE::FLOAT, 3},	// postion
		{ATTRIBUTE::FLOAT, 2}, // uv
	};

	std::vector<Vertex> vertex_data = {
		{vec3( 0.0,  0.0,  0.0), vec2(0.0, 0.0)},
		{vec3( 0.5,  0.5,  0.0), vec2(0.0, 1.0)},
		{vec3(-0.5, -0.5,  0.0), vec2(1.0, 1.0)}
	};

	VertexBuffer vertex_buffer({
		.attributes = vertex_attributes,
		.attributes_count = sizeof(vertex_attributes) / sizeof(Attributes),
		.data = vertex_data.data(),
		.count = (uint)vertex_data.size(),
	});



	// ----------------- Texture Buffer -----------------------------
	// Load the image
	int width, height, channels;
	unsigned char* texture_data = stbi_load("Assets/MoreClouds.png", &width, &height, &channels, 4);
	if (!texture_data) {
		std::cerr << "Failed to load image" << std::endl;
	}

	Texture2DBuffer texture({
		.size = uvec2(width, height),
		.format = TEXTURE_TYPE::RGBA8,
		.data = (void*)texture_data,
	});



	// ----------------- Render Pass -----------------------------
	RenderPass render_pass_main("Shaders/simple_triangle.glsl");



	// ----------------- Uniform Buffer Object -----------------------------
	struct Camera {
		mat4 view;
		mat4 projection;
	};

	Camera camera;
	UniformBuffer uniform_buffer({ (void*)&camera, sizeof(Camera) });



	// ----------------- Descriptor Set -----------------------------
	Descriptor descriptors[] = {
		{DESCRIPTOR_TYPE::UNIFORM_BUFFER,		(void*)&uniform_buffer, 	0, 0}, // bind = 0
		{DESCRIPTOR_TYPE::VERTEX_BUFFER_IN,		(void*)&vertex_buffer,		0, 0},
		{DESCRIPTOR_TYPE::TEXTURE_2D_BUFFER_IN,	(void*)&texture,			0, 0}, // location = 0
	};

	DescriptorSetBuffer descriptor_set({
		.descriptors = descriptors,
		.count = sizeof(descriptors) / sizeof(Descriptor),
	});



	// ----------------- Construct Frame Graph -----------------------------
	FrameGraph graph(window);

	// Render the geometry
	graph.addNode({
		.render_pass = &render_pass_main,
		.render_states = {},
		.descriptor_set = &descriptor_set,
	});

	graph.addNodeDisplay(); // Finally we display the frame

	graph.build();

	while (!window.isClosed()) {
		graph.run();
	}
}
*/
