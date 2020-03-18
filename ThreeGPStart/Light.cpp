#include "Light.h"

void Light::CreateLight(glm::vec4 position, glm::vec4 rotation, glm::vec3 colour, float attenuation, float ambientCoefficient, float coneAngle, glm::vec3 coneDirection)
{
	m_position = position;
	m_rotation = rotation;
	intensities = colour;
	m_attenuation = attenuation;
	m_ambientCoefficient = ambientCoefficient;
	m_coneAngle = coneAngle;
	m_coneDirection = coneDirection;
}

void Light::InitLight()
{
	
}

glm::vec4 Light::GetDirection()
{
	return m_rotation;
}

glm::vec4 Light::GetPosition()
{
	return m_position;
}

glm::vec3 Light::GetColour()
{
	return intensities;
}

float Light::GetAttenuation()
{
	return m_attenuation;
}

float Light::GetAmbientCoefficient()
{
	return m_ambientCoefficient;
}

float Light::GetConeAngle()
{
	return m_coneAngle;
}

glm::vec3 Light::GetConeDirection()
{
	return m_coneDirection;
}

void Light::Move(glm::vec3 moveDirection)
{
	m_position.x += moveDirection.x;
	m_position.y += moveDirection.y;
	m_position.z += moveDirection.z;
}
