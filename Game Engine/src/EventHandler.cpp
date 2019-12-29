#include "EventHandler.h"


EventHandler::EventHandler(GLFWwindow* window)
	:m_Window(window), m_Speed(0.05f), m_MouseSpeed(0.025f), m_xPos(0), m_yPos(0), m_xDiff(0), m_yDiff(0), m_HorizontalAngle(3.14f),
	m_VerticalAngle(0.0f), m_InitialFoV(45.0f), m_DeltaTime(0), m_Position(glm::vec3(0, 0, 5)), m_Direction(glm::vec3(0,0,0)),
	m_Up(glm::vec3(0, 0, 0))
{
}

EventHandler::~EventHandler()
{
}

void EventHandler::updateFromInputs()
{
	m_HorizontalAngle += m_MouseSpeed * m_DeltaTime * (float)m_xDiff;
	m_VerticalAngle += m_MouseSpeed * m_DeltaTime * (float)m_yDiff;

	double xpos, ypos;
	glfwGetCursorPos(m_Window, &xpos, &ypos);
	m_xDiff = m_xPos - xpos;
	m_yDiff = m_yPos - ypos;
	m_xPos = xpos;
	m_yPos = ypos;

	m_Direction = glm::vec3(cos(m_VerticalAngle) * sin(m_HorizontalAngle), sin(m_VerticalAngle), cos(m_VerticalAngle) * cos(m_HorizontalAngle));

	glm::vec3 right = glm::vec3(
		sin(m_HorizontalAngle - 3.14f / 2.0f),
		0,
		cos(m_HorizontalAngle - 3.14f / 2.0f)
	);

	m_Up = glm::cross(right, m_Direction);

	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS) {
		m_Position += m_Direction * m_DeltaTime * m_Speed;
	}
	// Move backward
	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS) {
		m_Position -= m_Direction * m_DeltaTime * m_Speed;
	}
	// Strafe right
	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS) {
		m_Position += right * m_DeltaTime * m_Speed;
	}
	// Strafe left
	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS) {
		m_Position -= right * m_DeltaTime * m_Speed;
	}

	//std::cout << m_HorizontalAngle << " : " << m_VerticalAngle << std::endl;
}

glm::mat4 EventHandler::getProjectionMatrix()
{
	return { glm::perspective(glm::radians(m_InitialFoV), 16.0f / 9.0f, 0.1f, 100.0f) };
}

glm::mat4 EventHandler::getViewMatrix()
{
	glm::mat4 ViewMatrix = glm::lookAt(
		m_Position,           // Camera is here
		m_Position + m_Direction, // and looks here : at the same position, plus "direction"
		m_Up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	return ViewMatrix;
}

