#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "../MBG/OpenGL/Window.hpp"

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

	Window example_window(800, 600);
	
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	while(!example_window.isClosed())
	{
		if (glfwGetKey(example_window.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(example_window.window, GLFW_TRUE);
		}

		glClear(GL_COLOR_BUFFER_BIT);
		example_window.pullAndSwapBuffers();
	}

    REQUIRE(1 + 1 == 2);
}