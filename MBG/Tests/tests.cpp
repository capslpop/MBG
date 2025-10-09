#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include "../MBG/OpenGL/Window.hpp"

TEST_CASE("Simple Triangle") {

    std::cout << "Hello TESTS!" << std::endl;

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