#include <iostream>
#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"

void dae::GameObject::Update(float deltaTime){
	for (const auto& component : m_components)
	{
		component->Update(deltaTime);
	}
}

void dae::GameObject::FixedUpdate(){
	for (const auto& component : m_components)
	{
		component->FixedUpdate();
	}
}

void dae::GameObject::Render() const
{
	if (m_texture)
	{
		const auto& pos = m_transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	}
	for (const auto& component : m_components)
	{
		component->Render();
	}
}

void dae::GameObject::addComponent(std::unique_ptr<dae::Component> component)
{
	m_components.push_back(std::move(component));
}

void dae::GameObject::removeComponent(dae::Component* component)
{
	m_components.erase(std::remove_if(m_components.begin(), m_components.end(), 
		[component](const std::unique_ptr<Component>& ptr) { return ptr.get() == component; }), 
		m_components.end());
}

dae::Component* dae::GameObject::getComponent(const std::type_info& typeInfo) const
{
	for (const auto& component : m_components)
	{
		const Component* ptr = component.get();
		if (typeid(*ptr) == typeInfo)
		{
			return component.get();
		}
	}
	return nullptr;
}

void dae::GameObject::hasComponentBeenAdded(dae::Component* component) const
{
	if (std::find_if(m_components.begin(), m_components.end(),
		[component](const std::unique_ptr<Component>& ptr) { return ptr.get() == component; }) != m_components.end())
	{
		std::cout << "Component has been added to the GameObject." << std::endl;
	}
	else
	{
		std::cout << "Component has not been added to the GameObject." << std::endl;
	}
}

void dae::GameObject::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.SetPosition(x, y, 0.0f);
}
