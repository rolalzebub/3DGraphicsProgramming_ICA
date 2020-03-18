#include "Renderer.h"
#include "ImageLoader.h"
#include "Skybox.h"

void Renderer::RenderObjects(Scene& scene)
{
	
	auto modelsToRender = scene.GetObjects();

	//Render models
	for (auto model : modelsToRender) {

		auto meshVector = model.GetMeshVector();

		for (int i = 0; i < meshVector.size(); i++) {
			glm::mat4 model_xform = glm::mat4(1);

			//Retrieve all the translations in the heirarchy and apply them to model_xform
			MeshNode* parentNode = meshVector[i]->GetParent();
			std::vector<MeshNode*> translations;
			while (parentNode != nullptr) {

				translations.push_back(parentNode);

				parentNode = parentNode->GetParent();
			}
			Helpers::CheckForGLError();
			for (MeshNode* node : translations) {
				model_xform = glm::translate(model_xform, node->GetTranslation());

				model_xform = glm::rotate(model_xform, glm::radians(node->GetRotation().x), glm::vec3(1, 0, 0));
				model_xform = glm::rotate(model_xform, glm::radians(node->GetRotation().y), glm::vec3(0, 1, 0));
				model_xform = glm::rotate(model_xform, glm::radians(node->GetRotation().z), glm::vec3(0, 0, 1));
			}

			model_xform = glm::translate(model_xform, meshVector[i]->GetTranslation());

			model_xform = glm::rotate(model_xform, glm::radians(meshVector[i]->GetRotation().x), glm::vec3(1, 0, 0));
			model_xform = glm::rotate(model_xform, glm::radians(meshVector[i]->GetRotation().y), glm::vec3(0, 1, 0));
			model_xform = glm::rotate(model_xform, glm::radians(meshVector[i]->GetRotation().z), glm::vec3(0, 0, 1));
			Helpers::CheckForGLError();
			GLuint model_xform_id = glGetUniformLocation(scene.GetProgram(), "model_xform");
			glUniformMatrix4fv(model_xform_id, 1, GL_FALSE, glm::value_ptr(model_xform));
			Helpers::CheckForGLError();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, meshVector[i]->GetTexID());
			GLuint id = glGetUniformLocation(scene.GetProgram(), "u_texture");
			Helpers::CheckForGLError();
			glUniform1i(id, 0);
			Helpers::CheckForGLError();

			glBindVertexArray(meshVector[i]->GetVAO());

			glDrawElements(GL_TRIANGLES, meshVector[i]->GetNumElements(), GL_UNSIGNED_INT, (void*)0);
			Helpers::CheckForGLError();
		}
	}
}

//void Renderer::RenderSkybox(Scene& scene, glm::mat4 projection_xform, glm::mat4 view_xform)
//{
//	Skybox* skybox;// = scene.GetSkybox();
//
//	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
//	//glUseProgram(skybox->GetProgram());
//
//	glUniformMatrix4fv(glGetUniformLocation(skybox->GetProgram(), "view_xform"), 1, GL_FALSE, glm::value_ptr(view_xform));
//	glUniformMatrix4fv(glGetUniformLocation(skybox->GetProgram(), "projection_xform"), 1, GL_FALSE, glm::value_ptr(projection_xform));
//
//	// skybox cube
//	glBindVertexArray(skybox->GetVAO());
//	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->GetTexID());
//	glDrawArrays(GL_TRIANGLES, 0, 36);
//	glBindVertexArray(0);
//	glDepthFunc(GL_LESS); // Set depth function back to default
//	Helpers::CheckForGLError();
//}

void Renderer::RenderTerrain(Scene& scene)
{
	for (auto terrain : scene.GetTerrain()) {

		glm::mat4 terrain_xform = glm::mat4(1);

		GLuint terrain_xform_id = glGetUniformLocation(scene.GetProgram(), "model_xform");
		glUniformMatrix4fv(terrain_xform_id, 1, GL_FALSE, glm::value_ptr(terrain_xform));
		Helpers::CheckForGLError();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, terrain->GetTexID());
		GLuint id = glGetUniformLocation(scene.GetProgram(), "u_texture");
		glUniform1i(id, 0);
		Helpers::CheckForGLError();

		glBindVertexArray(terrain->GetVAO());
		Helpers::CheckForGLError();
		glDrawElements(GL_TRIANGLES, terrain->GetNumElements(), GL_UNSIGNED_INT, (void*)0);
		Helpers::CheckForGLError();
	}
}

// Render the scene. Passed the delta time since last called.
void Renderer::Render(const Helpers::Camera& camera, float deltaTime, Scene& scene)
{
	// Configure pipeline settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Uncomment to render in wireframe (can be useful when debugging)
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Clear buffers from previous frame
	glClearColor(0.0f, 0.0f, 0.0f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// TODO: Compute viewport and projection matrix
	GLint viewportSizeMatrix[4];
	glGetIntegerv(GL_VIEWPORT, viewportSizeMatrix);
	const float aspect_ratio = viewportSizeMatrix[2] / (float)viewportSizeMatrix[3];
	glm::mat4 projection_xform = glm::perspective(glm::radians(45.0f), aspect_ratio, 1.f, 4000.f);

	// TODO: Compute camera view matrix and combine with projection matrix for passing to shader
	glm::mat4 view_xform = glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetLookVector(), camera.GetUpVector());
	glm::mat4 combined_xform = projection_xform * view_xform;

	// TODO: Send the combined matrix to the shader in a uniform
	glUseProgram(scene.GetProgram());
	GLuint combined_xform_id = glGetUniformLocation(scene.GetProgram(), "combined_xform");
	glUniformMatrix4fv(combined_xform_id, 1, GL_FALSE, glm::value_ptr(combined_xform));
	

	//TODO: Skybox
	RenderTerrain(scene);
	Helpers::CheckForGLError();
	RenderObjects(scene);
	Helpers::CheckForGLError();
	//RenderSkybox(scene, projection_xform, view_xform);

	// Always a good idea, when debugging at least, to check for GL errors
	Helpers::CheckForGLError();
}
