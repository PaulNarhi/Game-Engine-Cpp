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
#include "EventHandler.h"

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
		float cx = 1.0f;
		float cy = 1.0f;
		float cz = 1.0f;
		/* Data for vertex buffer*/
		float positions[] = {
			 // Side faces
			-0.5f+cx, -0.5f+cy, -5.0f+cz, 0.0f, 0.0f, //0
			 0.5f+cx, -0.5f+cy, -5.0f+cz, 0.5f, 0.0f, //1
			-0.5f+cx,  0.5f+cy, -5.0f+cz, 0.0f, 0.5f, //2
			 0.5f+cx,  0.5f+cy, -5.0f+cz, 0.5f, 0.5f, //3
		    -0.5f+cx, -0.5f+cy, -6.0f+cz, 0.5f, 0.0f, //4
			-0.5f+cx,  0.5f+cy, -6.0f+cz, 0.5f, 0.5f, //5
			 0.5f+cx, -0.5f+cy, -6.0f+cz, 0.0f, 0.0f, //6
			 0.5f+cx,  0.5f+cy, -6.0f+cz, 0.0f, 0.5f,  //7


			// Top face
			-0.5f+cx,  0.5f+cy, -5.0f+cz, 0.5f, 0.5f, //8
			 0.5f+cx,  0.5f+cy, -5.0f+cz, 1.0f, 0.5f, //9
			-0.5f+cx,  0.5f+cy, -6.0f+cz, 0.5f, 1.0f, //10
			 0.5f+cx,  0.5f+cy, -6.0f+cz, 1.0f, 1.0f, //11

			 // Bottom face
			-0.5f+cx,  -0.5f+cy, -5.0f+cz, 0.0f, 0.5f, //12
			 0.5f+cx,  -0.5f+cy, -5.0f+cz, 0.5f, 0.5f, //13
			-0.5f+cx,  -0.5f+cy, -6.0f+cz, 0.0f, 1.0f, //14
			 0.5f+cx,  -0.5f+cy, -6.0f+cz, 0.5f, 1.0f, //15
		};

		unsigned int indices[36] = {
			//sides
			0,   1,  2,
			1,   3,  2,
			0,   5,  4,
			0,   2,  5,
			1,   6,  3,
			7,   3,  6,
			4,   5,  6,
			6,   5,  7,
			8,   9, 10,
			10,  9, 11,
			13, 12, 14,
			13, 14, 15

			
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexBuffer vb(positions, 16 * 5 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);  //add mesh coordinates to vertex array
		layout.Push<float>(2);  //add texture coordinates to vertex array
		va.AddBuffer(vb, layout);

		/* Create an index buffer*/
		IndexBuffer ib(indices, 36);

		//glm::mat4 projMat = glm::ortho(-2.0f, 2.0f, -1.125f, 1.125f, -1.0f, 1.0f);
		///glm::mat4 projMat = glm::perspective(45.0f, 16.0f/9.0f, 1.0f, 150.0f);

		Shader shader = Shader("resources/shaders/Basic.shader");
		shader.Bind();
		//shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.9f, 1.0f);
		///shader.SetUniformMat4f("u_MVP", projMat);

		Texture texture = Texture("resources/textures/grass/grass.png");
		texture.Bind(); //Bind to slot 0 (modern machines have around 32 slots)
		shader.SetUniform1i("u_Texture", 0);
		
		//unbind all buffers
		va.Unbind();
		shader.Unbind();
		vb.Unbind();
		ib.Unbind();

		//create a new renderer
		Renderer renderer;


		

		EventHandler eventHandler(window);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


		//FPS
		double lastTime = glfwGetTime();
		int nofFrames = 0;

		/* Loop until the user closes the window */
		while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			glfwWindowShouldClose(window) == 0)
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
			
			
			/* Event handling system for basic movements*/
			eventHandler.GetDeltaTime(lastTime);
			eventHandler.updateFromInputs();

			
			eventHandler.updateFromInputs();
			glm::mat4 ProjectionMatrix = eventHandler.getProjectionMatrix();
			glm::mat4 ViewMatrix = eventHandler.getViewMatrix();
			glm::mat4 ModelMatrix = glm::mat4(1.0);
			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
			
			
			/* Render here */
			renderer.Clear();
			shader.Bind();

			shader.SetUniformMat4f("u_MVP", MVP);
			renderer.Draw(va, ib, shader);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}
	glfwTerminate();
	return 0;
}