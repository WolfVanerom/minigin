#pragma once
#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include "Transform.h"
#include "Component.h"
namespace dae
{

	class Texture2D;
	class GameObject final 
	{
		Transform m_transform{};
		std::shared_ptr<Texture2D> m_texture{};
		std::vector<std::unique_ptr<Component>> m_components{};
	public:
		void Update(float deltaTime);
		void FixedUpdate();
		void Render() const;
		void MarkForDeletion() { m_markedForDeletion = true; }

		void addComponent(std::unique_ptr<Component> component);
		void removeComponent(Component* component);
		Component* getComponent(const std::type_info& typeInfo) const;
		void hasComponentBeenAdded(Component* component) const;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		bool m_markedForDeletion{ false };

		GameObject() = default;
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
	};
}
