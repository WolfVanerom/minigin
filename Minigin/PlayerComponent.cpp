#include "PlayerComponent.h"

dae::PlayerComponent::PlayerComponent(GameObject* pOwner)
	:Component(pOwner)
{
	m_deathObserver = std::make_unique<deathObserver>();
	m_gameObjectDeathEvent->AddObserver(m_deathObserver.get());
}

dae::PlayerComponent::~PlayerComponent()
{
	if (m_gameObjectDeathEvent && m_deathObserver)
	{
		m_gameObjectDeathEvent->RemoveObserver(m_deathObserver.get());
	}
}

void dae::PlayerComponent::Update(float deltaTime)
{
	(void)deltaTime;
	const auto& worldPos = m_parent->GetWorldPosition();
	const int cellX = static_cast<int>(worldPos.x / LevelManager::m_tileWidth);
	const int cellY = static_cast<int>(worldPos.y / LevelManager::m_tileHeight);

	if (m_levelManager.GetCell(cellX, cellY) == LevelObjectType::emerald)
	{
		m_score += 100;
		m_levelManager.SetCell(cellX, cellY, LevelObjectType::empty);
		m_scoreChangedEvent->NotifyScoreChanged(m_parent);
	}
}

void dae::PlayerComponent::SubtractHealth(int amount)
{
	m_health -= amount;
	m_health = std::max(0, m_health);
	m_remainingLivesEvent->NotifyRemainingLivesChanged(m_parent);

	if (m_health <= 0)
	{
		m_gameObjectDeathEvent->NotifyPlayerDied(m_parent);
	}
}
