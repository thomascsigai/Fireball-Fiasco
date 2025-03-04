#include <EnemyFactory.h>

namespace DjipiApp
{
	EnemyFactory::EnemyFactory(std::vector<DjipiApp::Enemy>& enemies, Djipi::ResourceManager* resourceManager)
		: m_EnemiesRef(enemies), m_Gen(std::random_device{}()), ptr_ResourceManager(resourceManager)
	{
		m_TimerModifier = 0;
	}

	void EnemyFactory::Start()
	{
		m_RespawnTimer.Start();
		SpawnEnemy();
	}

	void EnemyFactory::Reset()
	{
		m_RespawnTimer.Stop();
		m_TimerModifier = 0;

		m_EnemiesRef.clear();
	}

	void EnemyFactory::Update(double deltaTime)
	{
		m_TimerModifier = SDL_GetTicks() / 50;

;		int modifier = std::clamp(ENEMY_BASE_SPAWN_TIME - (int)(m_TimerModifier), (int)ENEMY_MIN_SPAWN_TIME, (int)ENEMY_BASE_SPAWN_TIME);

		if (m_RespawnTimer.GetTicks() > modifier)
		{
			APP_LOG_INFO("Spawn " << m_RespawnTimer.GetTicks());
			m_RespawnTimer.Stop();
			SpawnEnemy();
			m_RespawnTimer.Start();
		}
	}

	void EnemyFactory::SpawnEnemy()
	{
		// Spawn enemy
		float posX, posY;
		std::uniform_int_distribution<int> xPosRange(-100, SCREEN_WIDTH + 100);
		std::uniform_int_distribution<int> yTopRange(-100, -10);
		std::uniform_int_distribution<int> yBottomRange(SCREEN_HEIGHT + 10, SCREEN_HEIGHT + 100);

		// yRange:
		// 0: Enemy spawn in top offside of the screen
		// 1: Enemy spawn in bottom offside of the screen
		std::uniform_int_distribution<int> yRange(0, 1);

		posX = xPosRange(m_Gen);
		yRange(m_Gen) == 0 ? posY = yTopRange(m_Gen) : posY = yBottomRange(m_Gen);

		m_EnemiesRef.emplace_back(Djipi::Vector2(posX, posY));

		if (ptr_ResourceManager != NULL)
		{
			m_EnemiesRef.back().SetTexture(ptr_ResourceManager->GetTexture("resources\\textures\\demon.png"));
		}
	}
}