#pragma once

#include "ExternalLibraryHeaders.h"
class Light
{
private:
	glm::vec4 m_position{ 0,0,0,0 };
	glm::vec4 m_rotation{ 0,0,0,0 };
	glm::vec3 intensities{ 1,1,1 }; //a.k.a. the color of the light
	float m_ambientCoefficient{ 0.2f };
	float m_attenuation{ 0.0f };
	float m_coneAngle{ 0.0f };
	glm::vec3 m_coneDirection{ 0,0,0 };

	

public:
	void CreateLight(glm::vec4 position, glm::vec4 rotation, glm::vec3 colour, float attenuation, float ambientCoefficient, float coneAngle = 0.0f, glm::vec3 coneDirection = glm::vec3(0));
	void InitLight();
	glm::vec4 GetDirection();
	glm::vec4 GetPosition();
	glm::vec3 GetColour();
	float GetAttenuation();
	float GetAmbientCoefficient();
	float GetConeAngle();
	glm::vec3 GetConeDirection();

	void Move(glm::vec3 moveDirection);
};

