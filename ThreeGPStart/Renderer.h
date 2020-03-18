#pragma once
#include "Scene.h"
class Renderer
{
private:
	void RenderObjects(Scene& scene);
	void RenderSkybox(Scene& scene, glm::mat4 projection_xform, glm::mat4 view_xform);
	void RenderTerrain(Scene& scene);

public:

	// Render the scene
	void Render(const Helpers::Camera& camera, float deltaTime, Scene& scene);
};

