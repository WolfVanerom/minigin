#pragma once
#include "Component.h"
#include <memory>

namespace dae
{
	class TextComponent;
	class Minigin;

	class FPSComponent final : public Component
	{
	public:
		void Update() override;
		void Render() const override;

		explicit FPSComponent(std::shared_ptr<GameObject> pOwner, std::shared_ptr<TextComponent> textComponent, Minigin* engine);
		virtual ~FPSComponent() = default;
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;
	private:
		std::weak_ptr<TextComponent> m_textComponent{};
		Minigin* m_engine{};
	};
}
