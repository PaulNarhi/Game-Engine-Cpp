#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>


#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1024, 576, "Game Engine", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* bind refresh interval to screen refresh rate*/
	glfwSwapInterval(1);

	/* Initialize GLEW and check ok*/
	if (glewInit() != GLEW_OK) {
		std::cout << " GLEW Error!" << std::endl;
	}

	/* Print openGL and driver version*/
	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		/* Data for vertex buffer*/
		float positions[20] = {
			-0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -1.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, -1.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, -1.0f, 1.0f, 1.0f
		};

		unsigned int indices[6] = {
			0, 1, 2,
			1, 2, 3
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexBuffer vb(positions, 4 * 5 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);  //add mesh coordinates to vertex array
		layout.Push<float>(2);  //add texture coordinates to vertex array
		va.AddBuffer(vb, layout);

		/* Create an index buffer*/
		IndexBuffer ib(indices, 6);

		//glm::mat4 projMat = glm::ortho(-2.0f, 2.0f, -1.125f, 1.125f, -1.0f, 1.0f);
		glm::mat4 projMat = glm::perspective(45.0f, 16.0f/9.0f, 1.0f, 150.0f);

		Shader shader = Shader("resources/shaders/Basic.shader");
		shader.Bind();
		//shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.9f, 1.0f);
		shader.SetUniformMat4f("u_MVP", projMat);

		Texture texture = Texture("resources/textures/grass/grass_side_small.png");
		texture.Bind(); //Bind to slot 0 (modern machines have around 32 slots)
		shader.SetUniform1i("u_Texture", 0);
		
		//unbind all buffers
		va.Unbind();
		shader.Unbind();
		vb.Unbind();
		ib.Unbind();

		Renderer renderer;

		float r = 0.0;
		float interval = 0.05;

		double lastTime = glfwGetTime();
		int nofFrames = 0;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* FPS counter*/
			double currentTime = glfwGetTime();
			nofFrames++;
			if (currentTime - lastTime >= 1.0)
			{
				std::cout << "FPS: " << 1 / ((1000.0 / double(nofFrames)) / 1000.0) << " ms/Frame: " << 1000.0/float(nofFrames) << std::endl;
				nofFrames = 0;
				lastTime += 1.0;
			}

			/* Render here */
			renderer.Clear();
			shader.Bind();
			//shader.SetUniform4f("u_Color", r, 0.3f, 0.9f, 1.0f);
			
			renderer.Draw(va, ib, shader);

			if (r > 1.0f)
				interval = -0.05f;
			else if (r < 0.0f)
				interval = 0.05f;

			r += interval;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}