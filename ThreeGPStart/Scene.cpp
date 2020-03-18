#include "Scene.h"
#include<iostream>
void Scene::CreateAquapig()
{
	std::vector<std::string> obj_filenames;		//order of addition to vector needs to follow heirarchy to form the vector of the correct order
	obj_filenames.push_back("Data/Models/AquaPig/hull.obj");
	obj_filenames.push_back("Data/Models/AquaPig/wing_right.obj");
	obj_filenames.push_back("Data/Models/AquaPig/wing_left.obj");
	obj_filenames.push_back("Data/Models/AquaPig/propeller.obj");
	obj_filenames.push_back("Data/Models/AquaPig/gun_base.obj");
	obj_filenames.push_back("Data/Models/AquaPig/gun.obj");

	MeshHeirarchy aquapig;
	aquapig.InitialiseModel(obj_filenames);

	aquapig.ConstructHeirarchyFromFile("Data/Models/AquaPig/aquaPig_Heirarchy.txt");
	aquapig.GetMeshVector()[0]->SetPosition(glm::vec3(0, 5, 0));
	//Set correct translation for bits of the aquapig
	aquapig.GetMeshVector()[1]->SetPosition(glm::vec3(-2.231, 0.272, -2.663));	//wing_right
	aquapig.GetMeshVector()[2]->SetPosition(glm::vec3(2.231, 0.272, -2.663));	//wing_left
	aquapig.GetMeshVector()[3]->SetPosition(glm::vec3(0, 1.395, -3.616));		//propeller position
	aquapig.GetMeshVector()[3]->SetRotation(glm::vec3(90.f, 0.f, 0.f));			//propeller rotation
	aquapig.GetMeshVector()[4]->SetPosition(glm::vec3(0, 0.569, -1.866));		//gun base
	aquapig.GetMeshVector()[5]->SetPosition(glm::vec3(0, 1.506, 0.644));		//gun

	sceneObjects.push_back(aquapig);

	sceneTerrains.push_back(std::make_shared<Terrain>());
}

void Scene::CreateLights()
{
	Light pointLight;
	Light directionalLight;
	Light coneLight, coneLight2, coneLight3, coneLight4;
	pointLight.CreateLight(glm::vec4(0, 10, 0, 1), glm::vec4(0, 0, 0, 0), glm::vec3(20,20,20), 0.4f, 0.0f, 0, glm::vec3(0));
	sceneLights.push_back(pointLight);
	directionalLight.CreateLight(glm::vec4(0, 0, 0, 0), glm::vec4(45, 0, 0, 0), glm::vec3(1, 1, 1), 0.4f, 0.2f, 0, glm::vec3(0));
	sceneLights.push_back(directionalLight);
	coneLight.CreateLight(glm::vec4(10, 10, 10, 1), glm::vec4(45, 0, 0, 1), glm::vec3(70, 30, 30), 0.4f, 0.0f, 30, glm::vec3(0, -1, 0));
	sceneLights.push_back(coneLight);
	coneLight2.CreateLight(glm::vec4(-10, 10, -10, 1), glm::vec4(45, 0, 0, 1), glm::vec3(20, 50, 70), 0.4f, 0.0f, 30, glm::vec3(0, -1, 0));
	sceneLights.push_back(coneLight2);
	coneLight3.CreateLight(glm::vec4(-10, 10, 10, 1), glm::vec4(45, 0, 0, 1), glm::vec3(60, 10, 80), 0.4f, 0.0f, 30, glm::vec3(0, -1, 0));
	sceneLights.push_back(coneLight3);
	coneLight4.CreateLight(glm::vec4(10, 10, -10, 1), glm::vec4(45, 0, 0, 1), glm::vec3(80, 80, 40), 0.4f, 0.0f, 30, glm::vec3(0, -1, 0));
	sceneLights.push_back(coneLight4);

	GLuint numLights_id = glGetUniformLocation(m_program, "numLights");
	glProgramUniform1i(m_program, numLights_id, sceneLights.size());


	for (int i = 0; i < sceneLights.size(); i++) {
		std::string lightName_rotation = "lightSource[" + std::to_string(i) +"].rotation";
		std::string lightName_position = "lightSource[" + std::to_string(i) + "].position";
		std::string lightName_colour = "lightSource[" + std::to_string(i) + "].colour_intensity";
		std::string lightName_attentuation = "lightSource[" + std::to_string(i) + "].attenuation";
		std::string lightName_ambient = "lightSource[" + std::to_string(i) + "].ambientCoefficient";
		std::string lightName_coneAngle = "lightSource[" + std::to_string(i) + "].coneAngle";
		std::string lightName_coneDirection = "lightSource[" + std::to_string(i) + "].coneDirection";

		GLuint lightPosition_id = glGetUniformLocation(m_program, lightName_position.c_str());
		GLuint lightDirection_id = glGetUniformLocation(m_program, lightName_rotation.c_str());
		GLuint lightColour_id = glGetUniformLocation(m_program, lightName_colour.c_str());
		GLuint lightAtten_id = glGetUniformLocation(m_program, lightName_attentuation.c_str());
		GLuint lightAmbient_id = glGetUniformLocation(m_program, lightName_ambient.c_str());
		GLuint lightConeAngle_id = glGetUniformLocation(m_program, lightName_coneAngle.c_str());
		GLuint lightConeDirection_id = glGetUniformLocation(m_program, lightName_coneDirection.c_str());
		Helpers::CheckForGLError();

		glProgramUniform4fv(m_program, lightDirection_id, 1, glm::value_ptr(sceneLights[i].GetDirection()));
		glProgramUniform4fv(m_program, lightPosition_id, 1, glm::value_ptr(sceneLights[i].GetPosition()));
		glProgramUniform3fv(m_program, lightColour_id, 1, glm::value_ptr(sceneLights[i].GetColour()));
		glProgramUniform1f(m_program, lightAtten_id, sceneLights[i].GetAttenuation());
		glProgramUniform1f(m_program, lightAmbient_id, sceneLights[i].GetAmbientCoefficient());
		Helpers::CheckForGLError();

	}
}

void Scene::CreateSkybox()
{
	std::vector<std::string> cubemap_faces;

	//in order: right, left, top, bottom, back, front
	cubemap_faces.push_back("Data/Sky/Clouds/SkyBox_Right.tga");
	cubemap_faces.push_back("Data/Sky/Clouds/SkyBox_Left.tga");
	cubemap_faces.push_back("Data/Sky/Clouds/SkyBox_Top.tga");
	cubemap_faces.push_back("Data/Sky/Clouds/SkyBox_Bottom.tga");
	cubemap_faces.push_back("Data/Sky/Clouds/SkyBox_Back.tga");
	cubemap_faces.push_back("Data/Sky/Clouds/SkyBox_Front.tga");

	//skybox.loadCubemap(cubemap_faces);
}


void Scene::InitSceneForRender()
{
	CreateProgram();

	CreateAquapig();

	CreateLights();

	//CreateSkybox();

	for (auto terrain : sceneTerrains) {
		terrain->InitializeTerrain();
	}
	for(auto light :sceneLights)
	 light.InitLight();
}

void Scene::AnimateAquapig(float deltaTime) {
	float RotationSpeed = 5000;

	sceneObjects[0].GetMeshVector()[3]->SetRotation(glm::vec3(90.f, sceneObjects[0].GetMeshVector()[3]->GetRotation().y + (RotationSpeed * deltaTime), 0.f));
}

void Scene::CreateProgram()
{
	// Create a new program (returns a unqiue id)
	m_program = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, "Data/Shaders/vertex_shader.glsl") };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, "Data/Shaders/fragment_shader.glsl") };
	if (vertex_shader == 0 || fragment_shader == 0)
		return;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(m_program, vertex_shader);

	// Attach the fragment shader (copies it)
	glAttachShader(m_program, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(m_program))
		return;

	Helpers::CheckForGLError();
}

void Scene::RenderUpdate(float deltaTime)
{
	AnimateAquapig(deltaTime);
}

const std::vector<Light>& Scene::GetLights()
{
	return sceneLights;
}

const std::vector<MeshHeirarchy>& Scene::GetObjects()
{
	return sceneObjects;
}

const std::vector<std::shared_ptr<Terrain>>& Scene::GetTerrain()
{
	return sceneTerrains;
}

//Skybox* Scene::GetSkybox()
//{
//	return &skybox;
//}

const GLuint Scene::GetProgram()
{
	return m_program;
}

void Scene::MoveLight(glm::vec3 moveDirection)
{
	sceneLights[0].Move(moveDirection);

	std::string lightName_position = "lightSource[0].position";

	GLuint lightPosition_id = glGetUniformLocation(m_program, lightName_position.c_str());

	glProgramUniform4fv(m_program, lightPosition_id, 1, glm::value_ptr(sceneLights[0].GetPosition()));
}


Scene::~Scene()
{
	glDeleteProgram(m_program);
}
