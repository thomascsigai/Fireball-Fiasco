#pragma once

#include <Djipi.h>
#include <Enemy.h>

#include <algorithm>
#include <random>

namespace DjipiApp
{
	class EnemyFactory
	{
	public:
		EnemyFactory(std::vector<DjipiApp::Enemy>& enemies, Djipi::ResourceManager* resourceManager = nullptr)
			: m_EnemiesRef(enemies), m_Gen(std::random_device{}()), ptr_ResourceManager(resourceManager)
		{
			m_RespawnTimer.Start();
			m_TimerModifier = 0;

			SpawnEnemy();
		}

		void Update(double deltaTime)
		{
			m_TimerModifier += 1;
			int modifier = std::clamp(ENEMY_BASE_SPAWN_TIME - (int)(m_TimerModifier / 300), (int)ENEMY_MIN_SPAWN_TIME, (int)ENEMY_BASE_SPAWN_TIME);

			if (m_RespawnTimer.GetTicks() > modifier)
			{
				m_RespawnTimer.Stop();
				SpawnEnemy();
				m_RespawnTimer.Start();
			}
		}
	private :
		std::vector<DjipiApp::Enemy>& m_EnemiesRef;
		Djipi::Timer m_RespawnTimer;

		Uint32 m_TimerModifier;
		std::mt19937 m_Gen;

		Djipi::ResourceManager* ptr_ResourceManager;

		void SpawnEnemy()
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
	};
}