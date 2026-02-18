#include "FPSComponent.h"
#include "TextComponent.h"
#include "Minigin.h"
#include <string>
#include <iomanip>
#include <sstream>

dae::FPSComponent::FPSComponent(std::shared_ptr<GameObject> pOwner, std::shared_ptr<TextComponent> textComponent, Minigin* engine)
	: Component(pOwner), m_textComponent(textComponent), m_engine(engine)
{
}

void dae::FPSComponent::Update()
{
	if (auto textComponent = m_textComponent.lock())
	{
		if (m_engine)
		{
			std::stringstream ss;
			ss << "FPS: " << std::fixed << std::setprecision(1) << m_engine->GetFPS();
			textComponent->SetText(ss.str());
		}
	}
}

void dae::FPSComponent::Render() const
{
}
