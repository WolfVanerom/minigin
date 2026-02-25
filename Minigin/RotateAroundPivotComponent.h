#pragma once
#include "Component.h"
#include <glm/glm.hpp>

namespace dae
{
	class RotateAroundPivotComponent final : public Component
	{
	public:
		void Update(float deltaTime) override;

		explicit RotateAroundPivotComponent(GameObject* pOwner, const glm::vec3& pivotPoint, float rotationSpeed);
		virtual ~RotateAroundPivotComponent() = default;
		RotateAroundPivotComponent(const RotateAroundPivotComponent& other) = delete;
		RotateAroundPivotComponent(RotateAroundPivotComponent&& other) = delete;
		RotateAroundPivotComponent& operator=(const RotateAroundPivotComponent& other) = delete;
		RotateAroundPivotComponent& operator=(RotateAroundPivotComponent&& other) = delete;
	private:
		glm::vec3 m_pivotPoint{};
		float m_rotationSpeed{ 0.f };

		void UpdatePivotToParent(GameObject* pObjectParent);
	};
}