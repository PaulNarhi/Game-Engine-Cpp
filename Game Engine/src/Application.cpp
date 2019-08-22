#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{

}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Initialize GLEW and check ok*/
	if (glewInit() != GLEW_OK) {
		std::cout << "Error!" << std::endl;
	}

	/* Print openGL and driver version*/
	std::cout << glGetString(GL_VERSION) << std::endl;
	
	/* Data for vertex buffer*/
	float positions[6] = {
		-0.25f, -0.25f,
		 0.25f, -0.25f,
		-0.25f,  0.25f
	};



	/* Create a vertex buffer*/
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW); //Last argument should be dynamic if shape is being modified in main loop. More on docs.gl


	/*tells openGL how data in vertex buffer is structured*/
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}