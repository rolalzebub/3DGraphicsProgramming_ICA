#pragma once

#include "MeshNode.h"

class MeshHeirarchy
{
private:
	std::vector<MeshNode*> m_meshVector;
	
public:
	std::vector<MeshNode*> GetMeshVector();
	bool LoadMeshPart(const std::string& model_filename);
	void ConstructHeirarchyFromFile(const std::string& heirarchy_filename);
	void InitialiseModel(const std::vector<std::string>& filenames);
	MeshHeirarchy() = default;
	~MeshHeirarchy() = default;
	MeshHeirarchy(MeshNode* node);
};

