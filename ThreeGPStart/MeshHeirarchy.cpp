#include "MeshHeirarchy.h"
#include <fstream>

std::vector<MeshNode*> MeshHeirarchy::GetMeshVector()
{
	return m_meshVector;
}

bool MeshHeirarchy::LoadMeshPart(const std::string& model_filename)
{
	Helpers::ModelLoader loader;

	if (!loader.LoadFromFile(model_filename))
		return false;

	for (Helpers::Mesh mesh : loader.GetMeshVector()) {

		MeshNode* node = new MeshNode(mesh);

		m_meshVector.push_back(node);
	}

	return true;
}

void MeshHeirarchy::ConstructHeirarchyFromFile(const std::string& heirarchy_filename)
{
	std::ifstream inputFileStream(heirarchy_filename);

	std::string line;

	int index;

	while (!inputFileStream.eof()) {

		std::getline(inputFileStream, line);

		index = std::atoi(&line[0]);
		
		for (int i = 1; i < line.length(); i++) {
			if (line[i] != ' ') {
				if (line[i] != ',') {

					//Set Parent-child for particular node
					int childIndex = std::atoi(&line[i]);
					m_meshVector[index]->AddChildren(m_meshVector[childIndex]);
					m_meshVector[childIndex]->SetParent(m_meshVector[index]);
				
				}
			}
		}
	}
	
	inputFileStream.close();
}

void MeshHeirarchy::InitialiseModel(const std::vector<std::string>& filenames)
{

	for (auto location : filenames) {
		if (!LoadMeshPart(location))
			std::cout << location + " didn't load \n";
	}

	for (int i = 0; i < m_meshVector.size(); i++)
	{
		m_meshVector[i]->InitialiseMesh();
	}
}

MeshHeirarchy::MeshHeirarchy(MeshNode* node)
{
	m_meshVector.push_back(node);
}

