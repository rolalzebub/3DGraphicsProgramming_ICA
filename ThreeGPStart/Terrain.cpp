#include "Terrain.h"

Terrain::Terrain(float tSize, int cells, int tileTexSize, int heightRange)
{
	size = tSize;
	numCells = cells;
	texture_tileSize = tileTexSize;
	terrainHeightRange = heightRange;
}

void Terrain::InitializeTerrain()
{
	std::vector<glm::vec3> m_vertexPositions;
	std::vector<glm::uint> m_elements;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec2> m_uvCoords;

	float cellSize = size / numCells;

	int numVerts_x = numCells + 1;
	int numVerts_z = numCells + 1;

	glm::vec3 start{ -size / 2, 0, size / 2 };
	ImageLoader heightmap;
	heightmap.Load(heightmap_filename);

	unsigned char* texels = (unsigned char*)heightmap.GetData();

	for (int z = 0; z < numVerts_z; z++) {
		for (int x = 0; x < numVerts_x; x++)
		{
			glm::vec3 pos{ start };

			pos.x += x * cellSize;
			pos.z -= z * cellSize;

			float u = (float)x / (numVerts_x - 1);
			float v = (float)z / (numVerts_z - 1);

			int heightMapX = (int)(u * (heightmap.Width() - 1));
			int heightMapY = (int)(v * (heightmap.Height() - 1));

			int offset = (heightMapY * heightmap.Width() + heightMapX) * 4;

			pos.y = texels[offset] * 0.03f;

			u *= texture_tileSize;
			v *= texture_tileSize;

			m_vertexPositions.push_back(pos);
			m_uvCoords.push_back(glm::vec2(u, v));
		}
	}


	bool toggle = true;
	for (int cellZ = 0; cellZ < numCells; cellZ++) {
		for (int cellX = 0; cellX < numCells; cellX++)
		{
			int startVertIndex = cellZ * numVerts_x + cellX;
			if (toggle) {
				//first triangle
				m_elements.push_back(startVertIndex);
				m_elements.push_back(startVertIndex + 1);
				m_elements.push_back(startVertIndex + numVerts_x + 1);

				//second triangle
				m_elements.push_back(startVertIndex);
				m_elements.push_back(startVertIndex + numVerts_x + 1);
				m_elements.push_back(startVertIndex + numVerts_x);
			}
			else
			{
				//first triangle
				m_elements.push_back(startVertIndex);
				m_elements.push_back(startVertIndex + 1);
				m_elements.push_back(startVertIndex + numVerts_x);

				//second triangle
				m_elements.push_back(startVertIndex + 1);
				m_elements.push_back(startVertIndex + numVerts_x + 1);
				m_elements.push_back(startVertIndex + numVerts_x);
			}
			toggle = !toggle;
		}
		toggle = !toggle;
	}


	m_normals.resize(m_vertexPositions.size());
	std::fill(m_normals.begin(), m_normals.end(), glm::vec3(0, 0, 0));

	for (int index = 0; index < m_elements.size(); index += 3) {

		int index1 = m_elements[index];
		int index2 = m_elements[index + 1];
		int index3 = m_elements[index + 2];

		glm::vec3 v0 = m_vertexPositions[index1];
		glm::vec3 v1 = m_vertexPositions[index2];
		glm::vec3 v2 = m_vertexPositions[index3];

		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;

		glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

		m_normals[index1] += normal;
		m_normals[index2] += normal;
		m_normals[index3] += normal;
	}

	for (glm::vec3& n : m_normals) {
		n = glm::normalize(n);
	}

	m_numElements += (GLuint) m_elements.size();

	ImageLoader tex_loader;
	tex_loader.Load(texture_filename);

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)tex_loader.Width(), (GLsizei)tex_loader.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_loader.GetData());
	glGenerateMipmap(GL_TEXTURE_2D);
	// Good idea to check for an error now:	
	Helpers::CheckForGLError();

	GLuint verticesVBO;
	glGenBuffers(1, &verticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_vertexPositions.size(), m_vertexPositions.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint UV_buffer;
	glGenBuffers(1, &UV_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, UV_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_uvCoords.size(), m_uvCoords.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint normalsBO;
	glGenBuffers(1, &normalsBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* m_normals.size(), m_normals.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	GLuint elementsVBO;
	glGenBuffers(1, &elementsVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_elements.size(), m_elements.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	//Helpers::CheckForGLError();
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	Helpers::CheckForGLError();
	
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, UV_buffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	Helpers::CheckForGLError();

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBO);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	Helpers::CheckForGLError();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsVBO);

	Helpers::CheckForGLError();


	// Clear VAO binding
	glBindVertexArray(0);

}

void Terrain::SetTextureFilename(const std::string& filename)
{
	texture_filename = filename;
}

void Terrain::SetHeightmapFilename(const std::string& filename)
{
	heightmap_filename = filename;
}

GLuint Terrain::GetVAO()
{
	return m_VAO;
}

GLuint Terrain::GetTexID()
{
	return m_textureID;
}

GLuint Terrain::GetNumElements()
{
	return m_numElements;
}

