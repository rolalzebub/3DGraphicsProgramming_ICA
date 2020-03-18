#include "MeshNode.h"

void MeshNode::InitialiseMesh()
{
	Helpers::ImageLoader image_loader;
	if (!image_loader.Load("Data/Models/AquaPig/aqua_pig_1K.png")) {
		std::cout << "aquapig texture failed to load";
		return;
	}
	std::cout << "aquapig texture loaded\n";

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)image_loader.Width(), (GLsizei)image_loader.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image_loader.GetData());
	glGenerateMipmap(GL_TEXTURE_2D);

	// Good idea to check for an error now:	
	Helpers::CheckForGLError();
	
	GLuint verticesVBO;
	glGenBuffers(1, &verticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_mesh.vertices.size(), m_mesh.vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint UV_buffer;
	glGenBuffers(1, &UV_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, UV_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_mesh.uvCoords.size(), m_mesh.uvCoords.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint elementsVBO;
	glGenBuffers(1, &elementsVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_mesh.elements.size(), m_mesh.elements.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_numElements += (GLuint)m_mesh.elements.size();

	GLuint normalsVBO;
	glGenBuffers(1, &normalsVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, normalsVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::vec3) * m_mesh.normals.size(), m_mesh.normals.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//Helpers::CheckForGLError();
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	Helpers::CheckForGLError();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsVBO);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, UV_buffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	Helpers::CheckForGLError();

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalsVBO);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// Clear VAO binding
	glBindVertexArray(0);
}

void MeshNode::AddChildren(MeshNode* node)
{
	m_children.push_back(node);
}

void MeshNode::SetParent(MeshNode* node)
{
	m_parent = node;
}

void MeshNode::SetPosition(glm::vec3 position)
{
	m_position = position;
}

void MeshNode::SetRotation(glm::vec3 rotation)
{
	m_rotation = rotation;
}

GLuint MeshNode::GetVAO()
{
	return m_VAO;
}

GLuint MeshNode::GetTexID()
{
	return m_textureID;
}

GLuint MeshNode::GetNumElements()
{
	return m_numElements;
}

glm::vec3 MeshNode::GetTranslation()
{
	return m_position;
}

glm::vec3 MeshNode::GetRotation()
{
	return m_rotation;
}

std::vector<MeshNode*> MeshNode::GetChildren()
{
	return m_children;
}

MeshNode* MeshNode::GetParent()
{
	return m_parent;
}

bool MeshNode::LoadMesh(const std::string& filename)
{
	Helpers::ModelLoader loader;

	std::vector<Helpers::Mesh> mesh;

	if (!loader.LoadFromFile(filename))
		return false;

	mesh = loader.GetMeshVector();
}

MeshNode::MeshNode(Helpers::Mesh mesh)
{
	m_mesh = mesh;
}
