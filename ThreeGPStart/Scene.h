#pragma once
#include "ExternalLibraryHeaders.h"
#include "Light.h"
#include "MeshHeirarchy.h"
#include "Terrain.h"
#include "Skybox.h"

class Scene
{
private:
	std::vector<Light> sceneLights;

	std::vector<MeshHeirarchy> sceneObjects;
	std::vector<std::shared_ptr<Terrain>> sceneTerrains;
	//Skybox skybox;
	// Program object - to host shaders
	GLuint m_program{ 0 };
	

	void CreateAquapig();
	void CreateLights();
	void CreateSkybox();
	void AnimateAquapig(float deltaTime);
	void CreateProgram();
public:
	void InitSceneForRender();
	void RenderUpdate(float deltaTime);
	const std::vector<Light>& GetLights();
	const std::vector<MeshHeirarchy>& GetObjects();
	const std::vector<std::shared_ptr<Terrain>>& GetTerrain();
	//Skybox* GetSkybox();
	const GLuint GetProgram();

	void MoveLight(glm::vec3 moveDirection);

	Scene() = default;
	~Scene();
};

