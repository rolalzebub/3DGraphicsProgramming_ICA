#include "Simulation.h"
#include "Renderer.h"
#include "Scene.h"

// Initialise this as well as the renderer, returns false on error
bool Simulation::Initialise()
{
	// Set up camera
	m_camera = std::make_shared<Helpers::Camera>();
	m_camera->Initialise(glm::vec3(0, 200, 900), glm::vec3(0)); // Jeep
	m_camera->Initialise(glm::vec3(-13.82f, 5.0f, 1.886f), glm::vec3(0.25f, 1.5f, 0), 30.0f,0.8f); // Aqua pig

	// Set up renderer
	m_renderer = std::make_shared<Renderer>();

	lit_scene.InitSceneForRender();
	
	return true;
}

// Handle any user input. Return false if program should close.
bool Simulation::HandleInput(GLFWwindow* window) 
{
	glm::vec3 m_currentMovement = glm::vec3(0);

	// You can get keypresses like this:
	// if (glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS) // means W key pressed
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		m_currentMovement.z += worldUnitsPerSecond;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		m_currentMovement.z -= worldUnitsPerSecond;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		m_currentMovement.x -= worldUnitsPerSecond;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		m_currentMovement.x += worldUnitsPerSecond;
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		m_currentMovement.y += worldUnitsPerSecond;
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
		m_currentMovement.y -= worldUnitsPerSecond;

	if (m_currentMovement != glm::vec3(0))
		lit_scene.MoveLight(m_currentMovement);

	// You can get mouse button input, returned state is GLFW_PRESS or GLFW_RELEASE
	// int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

	// Use this to get the mouse position:
	// double xpos, ypos;
	// glfwGetCursorPos(window, &xpos, &ypos);

	// To prevent the mouse leaving the window (and to hide the arrow) you can call:
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// To reenable it use GLFW_CURSOR_NORMAL

	// To see an example of input using GLFW see the camera.cpp file.
	return true;
}

// Update the simulation (and render) returns false if program should close
bool Simulation::Update(GLFWwindow* window)
{
	// Deal with any input
	if (!HandleInput(window))
		return false;

	// Calculate delta time since last called
	// We pass the delta time to the camera and renderer
	float timeNow = (float)glfwGetTime();
	float deltaTime{ timeNow - m_lastTime };
	m_lastTime = timeNow;

	// The camera needs updating to handle user input internally
	m_camera->Update(window, deltaTime);

	lit_scene.RenderUpdate(deltaTime);
	// Render the scene
	m_renderer->Render(*m_camera, deltaTime, lit_scene);

	return true;
}

std::shared_ptr<Helpers::Camera> Simulation::GetCamera()
{
	return m_camera;
}
