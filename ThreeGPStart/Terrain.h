#pragma once
#include "MeshHeirarchy.h"

class Terrain
{
private:

	float size{ 50 };

	int numCells{ 32 };

	int texture_tileSize{ 10 };

	int terrainHeightRange{ 10 };

	std::string texture_filename = "Data/Textures/grass11.bmp";
	std::string heightmap_filename = "Data/Textures/curvy.gif";

	//numElements
	GLuint m_numElements{ 0 };
	//textureID per mesh
	GLuint m_textureID{ 0 };
	//VAO per mesh
	GLuint m_VAO{ 0 };

public:
	Terrain() {};
	Terrain(float tSize, int cells, int tileTexSize, int heightRange);

	void InitializeTerrain();
	void SetTextureFilename(const std::string& filename);
	void SetHeightmapFilename(const std::string& filename);

	GLuint GetVAO();
	GLuint GetTexID();
	GLuint GetNumElements();
};