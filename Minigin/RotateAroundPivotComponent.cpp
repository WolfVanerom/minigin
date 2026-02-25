#include "RotateAroundPivotComponent.h"
#include "GameObject.h"

dae::RotateAroundPivotComponent::RotateAroundPivotComponent(GameObject* pOwner, const glm::vec3& pivotPoint, float rotationSpeed)
	: Component(pOwner)
	, m_pivotPoint(pivotPoint)
	, m_rotationSpeed(rotationSpeed)
{
}

void dae::RotateAroundPivotComponent::Update(float deltaTime)
{
	GameObject* pObjectParent = m_parent->GetParent();
	glm::vec3 currentPos;
	glm::vec3 pivotPoint = m_pivotPoint;

	if (pObjectParent)
	{
		UpdatePivotToParent(pObjectParent);
		currentPos = m_parent->GetWorldPosition() - pObjectParent->GetWorldPosition();
	}
	else
	{
		currentPos = m_parent->GetWorldPosition();
	}

	const float angle = m_rotationSpeed * deltaTime;
	glm::vec3 translatedPos = currentPos - pivotPoint;
	float cosAngle = cos(angle);
	float sinAngle = sin(angle);
	glm::vec3 rotatedPos;
	rotatedPos.x = translatedPos.x * cosAngle - translatedPos.y * sinAngle;
	rotatedPos.y = translatedPos.x * sinAngle + translatedPos.y * cosAngle;
	glm::vec3 newPos = rotatedPos + pivotPoint;
	m_parent->SetPosition(newPos.x, newPos.y, newPos.z);
}

void dae::RotateAroundPivotComponent::UpdatePivotToParent(GameObject* pObjectParent)
{
	if (pObjectParent)
	{
		m_pivotPoint = pObjectParent->GetLocalPosition() + (m_pivotPoint - pObjectParent->GetWorldPosition());
	}
}