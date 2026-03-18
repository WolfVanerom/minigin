#include "Observer.h"
#include <string>
#include "InputManager.h"
#include "PlayerComponent.h"
#include "TextComponent.h"

#if USE_STEAMWORKS
#include <steam_api.h>
#endif


namespace dae
{
	remainingLivesObserver::remainingLivesObserver(TextComponent* textComponent, PlayerComponent* playerCompoenent)
		: m_textComponent(textComponent), m_playerComponent(playerCompoenent)
	{
	}

	void deathObserver::OnNotify(Event event, GameObject* gameObject)
	{
		if (event == Event::PlayerDied && gameObject != nullptr)
		{
			InputManager::GetInstance().RemoveCommandsForGameObject(gameObject);
			gameObject->MarkForDeletion();
		}
	}

	scoreObserver::scoreObserver(TextComponent* textComponent, PlayerComponent* playerComponent)
		: m_textComponent(textComponent), m_playerComponent(playerComponent)
	{
	}

	void remainingLivesObserver::OnNotify(Event event, GameObject* gameObject)
	{
		if (event != Event::RemainingLivesChanged || gameObject == nullptr || m_textComponent == nullptr)
		{
			return;
		}

		if (m_playerComponent == nullptr)
		{
			return;
		}

		m_textComponent->SetText("#Lives = " + std::to_string(m_playerComponent->GetHealth()));
	}

	void scoreObserver::OnNotify(Event event, GameObject* gameObject)
	{
		if (event != Event::ScoreChanged || gameObject == nullptr || m_textComponent == nullptr)
		{
			return;
		}
		if (m_playerComponent == nullptr)
		{
			return;
		}

		int score = m_playerComponent->GetScore();

		if (!m_winnerAchievementUnlocked && score >= 500)
		{
			m_winnerAchievementUnlocked = true;
		#if USE_STEAMWORKS
			SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
			SteamUserStats()->StoreStats();
		#endif
		}

		m_textComponent->SetText("#Score = " + std::to_string(m_playerComponent->GetScore()));
	}
}
