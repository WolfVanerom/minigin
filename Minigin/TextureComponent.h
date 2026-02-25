#pragma once
#include "Component.h"
#include <string>

namespace dae
{
	class Texture2D;
	class TextureComponent final : public Component
	{
	public:
		void Render() const override;

		void SetTexture(const std::string& filename);

		explicit TextureComponent(GameObject* pOwner);
		virtual ~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;
	private:
		std::shared_ptr<Texture2D> m_texture{};
	};
}