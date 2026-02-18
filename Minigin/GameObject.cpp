#include <iostream>
#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Component.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(){
	for (const auto& component : m_components)
	{
		component->Update();
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

void dae::GameObject::addComponent(std::shared_ptr<dae::Component> component)
{
	m_components.push_back(component);
}

void dae::GameObject::removeComponent(std::shared_ptr<dae::Component> component)
{
	m_components.erase(std::remove(m_components.begin(), m_components.end(), component), m_components.end());
}

std::shared_ptr<dae::Component> dae::GameObject::getComponent(size_t index) const
{
	if (index < m_components.size()) {
		return m_components[index];
	}
	return nullptr;
}

void dae::GameObject::hasComponentBeenAdded(std::shared_ptr<dae::Component> component) const
{
	if (std::find(m_components.begin(), m_components.end(), component) != m_components.end())
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
