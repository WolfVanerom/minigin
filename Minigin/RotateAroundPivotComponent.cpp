#include "RotateAroundPivotComponent.h"
#include "GameObject.h"

dae::RotateAroundPivotComponent::RotateAroundPivotComponent(GameObject* pOwner, float rotationSpeed)
	: Component(pOwner)
	, m_rotationSpeed(rotationSpeed)
{
}

void dae::RotateAroundPivotComponent::Update(float deltaTime)
{
	glm::vec3 currentPos = m_parent->GetLocalPosition();

	const float angle = m_rotationSpeed * deltaTime;
	float cosAngle = cos(angle);
	float sinAngle = sin(angle);
	glm::vec3 rotatedPos{};
	rotatedPos.x = currentPos.x * cosAngle - currentPos.y * sinAngle;
	rotatedPos.y = currentPos.x * sinAngle + currentPos.y * cosAngle;
	m_parent->SetPosition(rotatedPos.x, rotatedPos.y, rotatedPos.z);
}