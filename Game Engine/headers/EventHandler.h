#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



class EventHandler
{
public:
	EventHandler(GLFWwindow* window);
	~EventHandler();

	GLFWwindow* m_Window;

	void updateFromInputs();
	glm::mat4 getProjectionMatrix();
	glm::mat4 getViewMatrix();

	inline void GetDeltaTime(float lastTime) { m_DeltaTime = glfwGetTime() - lastTime; }

private:
	double m_xPos, m_yPos;
	double m_xDiff, m_yDiff;
	float m_MouseSpeed;
	float m_Speed;
	float m_DeltaTime;
	float m_HorizontalAngle;
	float m_VerticalAngle;
	float m_InitialFoV;
	glm::vec3 m_Position;
	glm::vec3 m_Direction;
	glm::vec3 m_Up;
};