#pragma once

#include "ExternalLibraryHeaders.h"
#include "Scene.h"
class Renderer;
struct GLFWwindow;

// Simulation class to handle input, updating of the simulation and calling the renderer
class Simulation
{
private:
	//Lights for the scene
	Light point_light;

	Scene lit_scene;

	// A simple camera
	std::shared_ptr<Helpers::Camera> m_camera;

	// The renderer
	std::shared_ptr<Renderer> m_renderer;

	MeshHeirarchy aquapig;

	float worldUnitsPerSecond = 0.2f;

	// Remember last update time so we can calculate delta time
	float m_lastTime{ 0 };

	// Handle any user input. Return false if program should close.
	bool HandleInput(GLFWwindow* window);
public:
	// Initialise this as well as the renderer, returns false on error
	bool Initialise();	

	// Update the simulation (and render) returns false if program should clse
	bool Update(GLFWwindow* window);

	std::shared_ptr<Helpers::Camera> GetCamera();
};

