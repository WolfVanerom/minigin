#pragma once
#include "Component.h"
#include "Observer.h"
#include <memory>
#include "LevelManager.h"

namespace dae
{

	class PlayerComponent final : public Component
	{
	private:
		class PlayerDeathEventSubject final : public subject
		{
		public:
			void NotifyPlayerDied(GameObject* gameObject)
			{
				Notify(Event::PlayerDied, gameObject);
			}
		};

		class PlayerLivesEventSubject final : public subject
		{
		public:
			void NotifyRemainingLivesChanged(GameObject* gameObject)
			{
				Notify(Event::RemainingLivesChanged, gameObject);
			}
		};

		class scoreObserver : public subject
		{
		public:
			void NotifyScoreChanged(GameObject* gameObject)
			{
				Notify(Event::ScoreChanged, gameObject);
			}
		};

		int m_health{ 5 };
		int m_score{ 0 };

		std::unique_ptr<PlayerDeathEventSubject> m_gameObjectDeathEvent{ std::make_unique<PlayerDeathEventSubject>() };
		std::unique_ptr<PlayerLivesEventSubject> m_remainingLivesEvent{ std::make_unique<PlayerLivesEventSubject>() };
		std::unique_ptr<scoreObserver> m_scoreChangedEvent{ std::make_unique<scoreObserver>() };
		std::unique_ptr<deathObserver> m_deathObserver{};

		LevelManager& m_levelManager{ LevelManager::GetInstance() };
	public:
		PlayerComponent(GameObject* pOwner);
		~PlayerComponent() override;
		PlayerComponent(const PlayerComponent& other) = delete;
		PlayerComponent(PlayerComponent&& other) = delete;
		PlayerComponent& operator=(const PlayerComponent& other) = delete;
		PlayerComponent& operator=(PlayerComponent&& other) = delete;
		void Update(float deltaTime) override;

		void SubtractHealth(int amount);
		int GetHealth() const { return m_health; }
		int GetScore() const { return m_score; }
		void AddLivesObserver(Observer* observer) { m_remainingLivesEvent->AddObserver(observer); }
		void AddScoreObserver(Observer* observer) { m_scoreChangedEvent->AddObserver(observer); }
		void RemoveLivesObserver(Observer* observer) { m_remainingLivesEvent->RemoveObserver(observer); }
		void RemoveScoreObserver(Observer* observer) { m_scoreChangedEvent->RemoveObserver(observer); }
		void AddDeathObserver(Observer* observer) { m_gameObjectDeathEvent->AddObserver(observer); }
	};
}