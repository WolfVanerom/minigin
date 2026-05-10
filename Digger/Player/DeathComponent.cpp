#include "DeathComponent.h"
#include <SDL3/SDL_log.h>

namespace dae {
	DeathComponent::DeathComponent(GameObject* pOwner)
		: Component{ pOwner }
	{
	}
	void DeathComponent::Update(float) {
	}
	void DeathComponent::OnNotify(Event event, GameObject*)
	{
		if (event != Event::PlayerDied)
			return;
		m_parent->MarkForDeletion();
	}
}