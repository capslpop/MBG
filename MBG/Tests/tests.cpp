#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "../MBG/OpenGL/Window.hpp"
#include "../MBG/OpenGL/VertexBuffer.hpp"
#include "../MBG/OpenGL/VertexElementBuffer.hpp"
#include "../MBG/OpenGL/Texture3D.hpp"

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstdlib>
#include <cstring>

static GLuint get_shader_program_from_file(const char* vertex_shader_source_file_name, const char* fragment_shader_source_file_name)
{
	enum
	{
		MAX_SIZE_BUFFER_LINE_FROM_FILE = 1024
	};
	char buffer_line_from_file[MAX_SIZE_BUFFER_LINE_FROM_FILE] = { '\0' };

	char* vertex_shader_source_code = nullptr;
	char* fragment_shader_source_code = nullptr;

	GLuint shader_program = 0;
	GLuint vertex_shader_object = 0;
	GLuint fragment_shader_object = 0;

	FILE* vertex_shader_source_file_stream = fopen(vertex_shader_source_file_name, "r");
	FILE* fragment_shader_source_file_stream = fopen(fragment_shader_source_file_name, "r");

	size_t max_size_vertex_shader_source_code = 4096;
	size_t current_size_vertex_shader_source_code = 0;

	size_t max_size_fragment_shader_source_code = 4096;
	size_t current_size_fragment_shader_source_code = 0;

	GLint param_compile_success_vertex_shader_object = GL_FALSE;
	GLint param_compile_success_fragment_shader_object = GL_FALSE;

	GLint param_link_success_shader_program = GL_FALSE;

	if (vertex_shader_source_file_stream == NULL)
	{
		fprintf(stderr, "get_shader_program_from_file(const char*, const char*): Failed to open the vertex shader source file.\n");
		goto QUIT_GET_SHADER_PROGRAM_FROM_FILE;
	}
	if (fragment_shader_source_file_stream == NULL)
	{
		fprintf(stderr, "get_shader_program_from_file(const char*, const char*): Failed to open the fragment shader source file.\n");
		goto QUIT_GET_SHADER_PROGRAM_FROM_FILE;
	}

	vertex_shader_source_code = (char*)malloc(max_size_vertex_shader_source_code);

	while (fgets(buffer_line_from_file, MAX_SIZE_BUFFER_LINE_FROM_FILE, vertex_shader_source_file_stream) != NULL)
	{
		const size_t length_buffer_line_from_file = strlen(buffer_line_from_file);
		const size_t new_size_vertex_shader_source_code = current_size_vertex_shader_source_code + length_buffer_line_from_file;
		const bool b_vertex_shader_source_code_overflows = new_size_vertex_shader_source_code > max_size_vertex_shader_source_code; // the length for the null character is already included.

		if (b_vertex_shader_source_code_overflows)
		{
			char* const temporary_vertex_shader_source_code = vertex_shader_source_code;
			vertex_shader_source_code = (char*)malloc(new_size_vertex_shader_source_code + 4096);
			strcpy(vertex_shader_source_code, temporary_vertex_shader_source_code);

			free(temporary_vertex_shader_source_code);
		}
		else
		{
			strcpy(vertex_shader_source_code + current_size_vertex_shader_source_code, buffer_line_from_file);
		}

		current_size_vertex_shader_source_code = new_size_vertex_shader_source_code;
		max_size_vertex_shader_source_code = new_size_vertex_shader_source_code + 4096;
	}
#ifdef _DEBUG
	fprintf(stderr, "get_shader_program_from_file(const char*, const char*): <GL_VERTEX_SHADER> %s:\n%s\n", vertex_shader_source_file_name, vertex_shader_source_code);
#endif

	fragment_shader_source_code = (char*)malloc(max_size_fragment_shader_source_code);

	while (fgets(buffer_line_from_file, MAX_SIZE_BUFFER_LINE_FROM_FILE, fragment_shader_source_file_stream) != NULL)
	{
		const size_t length_buffer_line_from_file = strlen(buffer_line_from_file);
		const size_t new_size_fragment_shader_source_code = current_size_fragment_shader_source_code + length_buffer_line_from_file;
		const bool b_fragment_shader_source_code_overflows = new_size_fragment_shader_source_code > max_size_fragment_shader_source_code; // the length for the null character is already included.

		if (b_fragment_shader_source_code_overflows)
		{
			char* const temporary_fragment_shader_source_code = fragment_shader_source_code;
			fragment_shader_source_code = (char*)malloc(new_size_fragment_shader_source_code + 4096);
			strcpy(fragment_shader_source_code, temporary_fragment_shader_source_code);

			free(temporary_fragment_shader_source_code);
		}
		else
		{
			strcpy(fragment_shader_source_code + current_size_fragment_shader_source_code, buffer_line_from_file);
		}

		current_size_fragment_shader_source_code = new_size_fragment_shader_source_code;
		max_size_fragment_shader_source_code = new_size_fragment_shader_source_code + 4096;
	}
#ifdef _DEBUG
	fprintf(stderr, "get_shader_program_from_file(const char*, const char*): <GL_FRAGMENT_SHADER> %s:\n%s\n", fragment_shader_source_file_name, fragment_shader_source_code);
#endif

	vertex_shader_object = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader_object, 1, &vertex_shader_source_code, nullptr);
	glCompileShader(vertex_shader_object);
	glGetShaderiv(vertex_shader_object, GL_COMPILE_STATUS, &param_compile_success_vertex_shader_object);
	if (param_compile_success_vertex_shader_object == GL_FALSE)
	{
		char info_log_vertex_shader[1024] = { '\0' };
		glGetShaderInfoLog(vertex_shader_object, 1024, nullptr, info_log_vertex_shader);
		fprintf(stderr, "get_shader_program_from_file(const char*, const char*): Failed to compile the vertex shader %s.\n%s\n", vertex_shader_source_file_name, info_log_vertex_shader);
		goto QUIT_GET_SHADER_PROGRAM_FROM_FILE;
	}

	fragment_shader_object = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader_object, 1, &fragment_shader_source_code, nullptr);
	glCompileShader(fragment_shader_object);
	glGetShaderiv(fragment_shader_object, GL_COMPILE_STATUS, &param_compile_success_fragment_shader_object);
	if (param_compile_success_fragment_shader_object == GL_FALSE)
	{
		char info_log_fragment_shader[1024] = { '\0' };
		glGetShaderInfoLog(fragment_shader_object, 1024, nullptr, info_log_fragment_shader);
		fprintf(stderr, "get_shader_program_from_file(const char*, const char*): Failed to compile the fragment shader %s.\n%s\n", fragment_shader_source_file_name, info_log_fragment_shader);
		goto QUIT_GET_SHADER_PROGRAM_FROM_FILE;
	}

	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader_object);
	glAttachShader(shader_program, fragment_shader_object);
	glLinkProgram(shader_program);
	glGetProgramiv(shader_program, GL_LINK_STATUS, &param_link_success_shader_program);
	if (param_link_success_shader_program == GL_FALSE)
	{
		char info_log_shader_program[1024] = { '\0' };
		glGetProgramInfoLog(fragment_shader_object, 1024, nullptr, info_log_shader_program);
		fprintf(stderr, "get_shader_program_from_file(const char*, const char*): Failed to link the shader program.\n%s\n", info_log_shader_program);
		goto QUIT_GET_SHADER_PROGRAM_FROM_FILE;
	}

QUIT_GET_SHADER_PROGRAM_FROM_FILE:
	if (shader_program != 0)
	{
		if (vertex_shader_object != 0)
		{
			glDetachShader(shader_program, vertex_shader_object);
		}
		if (fragment_shader_object != 0)
		{
			glDetachShader(shader_program, fragment_shader_object);
		}
	}

	if (vertex_shader_object != 0)
	{
		glDeleteShader(vertex_shader_object);
	}
	if (fragment_shader_object != 0)
	{
		glDeleteShader(fragment_shader_object);
	}

	free(vertex_shader_source_code);
	free(fragment_shader_source_code);
	fclose(vertex_shader_source_file_stream);
	fclose(fragment_shader_source_file_stream);

	return shader_program;
}

TEST_CASE("Simple Triangle") {
    std::cout << "Hello TESTS!" << std::endl;

	// This is what a triangle drawing application will look like

	//  Window example_window(800, 600);
	//  
	//  struct Vertex {
	//  	float x;
	//  	float y;
	//  };
	//  
	//  Vertex data[] = {
	//  	Vertex(0.0, 0.0),
	//  	Vertex(0.5, 0.5),
	//  	Vertex(-0.5, -0.5),
	//  };
	//  
	//  VertexBuffer vertex_buffer(sizeof(data), sizeof(Vertex), "vertex_buffer", data);
	//  vertex_buffer.addFloat(2); // states that there are 2 floats
	//  // add more if needed
	//  vertex_buffer.link();
	//  
	//  RenderPass main();
	//  main.memory(vertex_buffer);
	//  main.addVertexShader("C:\");
	//  main.addFragmentShader();
	//  main.link();
	//  
	//  
	//  
	//  while (!example_window.isClosed()) {
	//	FrameGraph graph;
	//		graph.RenderPassNode({
	//  	&main
	//  });
	//  graph.compile();
	//  	graph.run();
	//  }

    REQUIRE(1 + 1 == 2);
}

TEST_CASE("Simple Window") {

	std::cout << "Hello Window!" << std::endl;

	Window example_window(800, 600, "Somebody Touched My Spaghet On The Moon");

	GLfloat vertices_triangle[] = {
		// vertices			// colors
		-0.5, 0.5, 0.0,		1.0f, 0.0f, 0.0f,
		0.5, 0.5, 0.0,		0.0f, 1.0f, 0.0f,
		-0.5, -0.5, 0.0,	0.0f, 0.0f, 1.0f,
		0.5, -0.5, 0.0,		0.8f, 0.6f, 0.4f
	};

	GLuint indices_triangle[] = {
		0, 1, 2,
		1, 2, 3
	};

	VertexElementBuffer triangle_element_buffer(vertices_triangle, sizeof(vertices_triangle), indices_triangle, sizeof(indices_triangle), "triangleEBO", GL_STATIC_DRAW);	
	triangle_element_buffer.BeginAttrib();
	triangle_element_buffer.AddFloat(3);
	triangle_element_buffer.AddFloat(3);
	triangle_element_buffer.EndAttrib();
	triangle_element_buffer.BeginAttrib();
	triangle_element_buffer.AddFloat(3);
	triangle_element_buffer.EndAttrib();
	triangle_element_buffer.BeginAttrib();
	triangle_element_buffer.AddFloat(3);
	triangle_element_buffer.AddFloat(3);
	triangle_element_buffer.EndAttrib();
	triangle_element_buffer.BeginAttrib();
	triangle_element_buffer.AddFloat(3);
	triangle_element_buffer.EndAttrib();
	triangle_element_buffer.BeginAttrib();
	triangle_element_buffer.AddFloat(3);
	triangle_element_buffer.AddFloat(3);
	triangle_element_buffer.EndAttrib();

	GLuint program_draw_triangle = get_shader_program_from_file("Shaders/vert_triangle.glsl", "Shaders/frag_triangle.glsl");

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	while(!example_window.isClosed())
	{
		if (glfwGetKey(example_window.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(example_window.window, GLFW_TRUE);
		}

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program_draw_triangle);
		//triangle_vertex_buffer.Bind();
		triangle_element_buffer.Bind();
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		example_window.pullAndSwapBuffers();
	}

    REQUIRE(1 + 1 == 2);
}

TEST_CASE("Simple Texture") {

	std::cout << "Hello Texture!" << std::endl;

	Window example_window(800, 600, "3D Texture Test");

	GLfloat vertices_triangle[] = {
		// vertices			// colors		  // textures
		-0.5, 0.5, 0.0,		1.0f, 0.0f, 0.0f, 0.0, 1.0,
		0.5, 0.5, 0.0,		0.0f, 1.0f, 0.0f, 1.0, 1.0,
		-0.5, -0.5, 0.0,	0.0f, 0.0f, 1.0f, 0.0, 0.0,
		0.5, -0.5, 0.0,		0.8f, 0.6f, 0.4f, 1.0, 0.0
	};

	GLuint indices_triangle[] = {
		0, 1, 2,
		1, 2, 3
	};

	VertexElementBuffer triangle_element_buffer(vertices_triangle, sizeof(vertices_triangle), indices_triangle, sizeof(indices_triangle), "triangleEBO", GL_STATIC_DRAW);
	triangle_element_buffer.BeginAttrib();
	triangle_element_buffer.AddFloat(3);
	triangle_element_buffer.AddFloat(3);
	triangle_element_buffer.EndAttrib();
	triangle_element_buffer.BeginAttrib();
	triangle_element_buffer.AddFloat(3);
	triangle_element_buffer.EndAttrib();
	triangle_element_buffer.BeginAttrib();
	triangle_element_buffer.AddFloat(3);
	triangle_element_buffer.AddFloat(3);
	triangle_element_buffer.EndAttrib();
	triangle_element_buffer.BeginAttrib();
	triangle_element_buffer.AddFloat(3);
	triangle_element_buffer.EndAttrib();
	triangle_element_buffer.BeginAttrib();
	triangle_element_buffer.AddFloat(3);
	triangle_element_buffer.AddFloat(3);
	triangle_element_buffer.EndAttrib();

	//Add 3 attributes: 3 floats for vertices, 3 floats for colors, 2 floats for textures
	triangle_element_buffer.BeginAttrib();
	triangle_element_buffer.AddFloat(3);
	triangle_element_buffer.AddFloat(3);
	triangle_element_buffer.AddFloat(2);
	triangle_element_buffer.EndAttrib();

	GLuint program_draw_triangle = get_shader_program_from_file("Shaders/vert_texture.glsl", "Shaders/frag_texture.glsl");
	
	// Generate 3D LUT for testing
	
	// Sets texture size (e.x. LUT_SIZE*LUT_SUZE*LUT_SIZE)
	const int LUT_SIZE = 64;

	// Create vector of floats (this makes the data type GL_FLOAT)
	std::vector<float> lut(LUT_SIZE * LUT_SIZE * LUT_SIZE * 4);

	// Generate each pixel
	for (int b = 0; b < LUT_SIZE; ++b) {
		for (int g = 0; g < LUT_SIZE; ++g) {
			for (int r = 0; r < LUT_SIZE; ++r) {

				float r_float = r / float(LUT_SIZE - 1);
				float g_float = g / float(LUT_SIZE - 1);
				float b_float = b / float(LUT_SIZE - 1);

				int index = ((b * LUT_SIZE + g) * LUT_SIZE + r) * 4;

				lut[index + 0] = r_float; // R
				lut[index + 1] = g_float; // G
				lut[index + 2] = b_float; // B
				lut[index + 3] = 1.0;	 // A
			}
		}
	}


	TextureParams params(LUT_SIZE, LUT_SIZE, LUT_SIZE, TEXTURE_TYPE::RGBA16F, GL_FLOAT);

	Texture3D test_texture_data(params);

	glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, LUT_SIZE, LUT_SIZE, LUT_SIZE, GL_RGBA, GL_FLOAT, lut.data());


	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	while (!example_window.isClosed())
	{
		//Get changing sin^2(x) using time
		float sin_sq_time = sin((float)glfwGetTime()) * sin((float)glfwGetTime());

		if (glfwGetKey(example_window.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(example_window.window, GLFW_TRUE);
		}

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program_draw_triangle);

		//Add uniform to program
		glUniform1i(glGetUniformLocation(program_draw_triangle, "test_texture_data"), 0);
		glUniform1f(glGetUniformLocation(program_draw_triangle, "sin_sq_time"), sin_sq_time);
		triangle_element_buffer.Bind();
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		example_window.pullAndSwapBuffers();
	}

	REQUIRE(1 + 1 == 2);
}