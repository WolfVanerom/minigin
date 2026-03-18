#pragma once
#include "GameObject.h"
#include <algorithm>
#include <vector>

namespace dae {

	enum class Event
	{
		None,
		RemainingLivesChanged,
		PlayerDied,
		EnemyDefeated,
		ScoreChanged,
		LevelCompleted
	};

	struct Observer
	{
		virtual ~Observer() = default;
		virtual void OnNotify(Event event, GameObject* gameObject) = 0;
	};

	class subject
	{
	public:
		void AddObserver(Observer* observer)
		{
			m_observers.push_back(observer);
		}
		void RemoveObserver(Observer* observer)
		{
			m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end());
		}
	protected:
		void Notify(Event event, GameObject* gameObject)
		{
			for (Observer* observer : m_observers)
			{
				observer->OnNotify(event, gameObject);
			}
		}
	private:
		std::vector<Observer*> m_observers;
	};

	class TextComponent;
	class PlayerComponent;

	class remainingLivesObserver : public Observer
	{
	public:
		explicit remainingLivesObserver(TextComponent* textComponent, PlayerComponent* playerComponent);
		void OnNotify(Event event, GameObject* gameObject) override;
	private:
		TextComponent* m_textComponent{};
		PlayerComponent* m_playerComponent{};
	};

	class deathObserver : public Observer
	{
	public:
     void OnNotify(Event event, GameObject* gameObject) override;
	};

	class scoreObserver : public Observer
	{
	public:
		explicit scoreObserver(TextComponent* textComponent, PlayerComponent* playerComponent);
		void OnNotify(Event event, GameObject* gameObject) override;
	private:
		TextComponent* m_textComponent{};
		PlayerComponent* m_playerComponent{};
		bool m_winnerAchievementUnlocked{ false };
	};
}