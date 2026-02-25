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

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if (IsChild(parent) || parent == this || m_parent == parent) {
		return;
	}
	if (parent == nullptr) {
		SetLocalPosition(GetWorldPosition());
	}
	else
	{
		if (keepWorldPosition) {
			SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
		}
		SetPositionDirty();
	}
	if (m_parent) {
		m_parent->RemoveChild(this);
	}
	m_parent = parent;
	if (m_parent) {
		m_parent->AddChild(this);
	}
}

void dae::GameObject::AddChild(GameObject* child)
{
	if (child == nullptr || IsChild(child) || child == this) {
		return;
	}
	child->SetParent(this, true);
	m_children.push_back(child);
}

void dae::GameObject::RemoveChild(GameObject* child)
{
	m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
}

void dae::GameObject::SetPosition(float x, float y, float z)
{
	SetLocalPosition(glm::vec3(x, y, z));
}

void dae::GameObject::SetLocalPosition(const glm::vec3& pos)
{
	m_localTransform.SetPosition(pos);
	SetPositionDirty();
}

const glm::vec3& dae::GameObject::GetWorldPosition()
{
	if (m_positionDirty) {
		UpdateWorldPosition();
	}
	return m_worldTransform.GetPosition();
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_positionDirty)
	{
		if (m_parent == nullptr) {
			m_worldTransform.SetPosition(m_localTransform.GetPosition());
		}
		else {
			m_worldTransform.SetPosition(m_parent->GetWorldPosition() + m_localTransform.GetPosition());
		}
	}
	m_positionDirty = false;
}

bool dae::GameObject::IsChild(const GameObject* potentialChild) const
{
	for (const auto& child : m_children)
	{
		if (child == potentialChild || child->IsChild(potentialChild))
		{
			return true;
		}
	}
	return false;
}