#include "TextureComponent.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"

dae::TextureComponent::TextureComponent(GameObject* pOwner)
	: Component(pOwner)
{
}

void dae::TextureComponent::Render() const
{
	if (m_texture)
	{
		const auto& pos = m_parent->GetWorldPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	}
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}