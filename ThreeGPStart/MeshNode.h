#pragma once
#include "ExternalLibraryHeaders.h"
#include "ImageLoader.h"
#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"
using namespace Helpers;

class MeshNode
{
private:

	Helpers::Mesh m_mesh;
	glm::vec3 m_position{ 0, 0, 0 }; //Relative to parent
	glm::vec3 m_rotation{ 0.f, 0.f, 0.f }; //Relative to parent
	//numElements per mesh
	GLuint m_numElements{ 0 };
	//textureID per mesh
	GLuint m_textureID{ 0 };
	//VAO per mesh
	GLuint m_VAO{ 0 };
	//pointer to parent
	MeshNode* m_parent{ nullptr };
	//vector of pointers to children
	std::vector<MeshNode*> m_children;

public:

	void InitialiseMesh();
	void AddChildren(MeshNode* node);
	void SetParent(MeshNode* node);
	void SetPosition(glm::vec3 position);
	void SetRotation(glm::vec3 rotation);
	GLuint GetVAO();
	GLuint GetTexID();
	GLuint GetNumElements();
	glm::vec3 GetTranslation();
	glm::vec3 GetRotation();
	std::vector<MeshNode*> GetChildren();
	MeshNode* GetParent();
	bool LoadMesh(const std::string& filename);
	MeshNode() = default;
	~MeshNode() = default;
	MeshNode(Helpers::Mesh mesh);

};

