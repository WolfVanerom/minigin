#pragma once
#include <string>
#include <memory>
#include "Transform.h"
namespace dae
{

	class Component;
	class Texture2D;
	class GameObject final 
	{
		Transform m_transform{};
		std::shared_ptr<Texture2D> m_texture{};
		std::vector<std::shared_ptr<Component>> m_components{};
	public:
		void Update();
		void FixedUpdate();
		void Render() const;

		void addComponent(std::shared_ptr<Component> component);
		void removeComponent(std::shared_ptr<Component> component);
		std::shared_ptr<Component> getComponent(size_t index) const;
		void hasComponentBeenAdded(std::shared_ptr<Component> component) const;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
	};
}
