#pragma once

#include <Djipi.h>
#include <Enemy.h>

#include <algorithm>

namespace DjipiApp
{
	class EnemyFactory
	{
	public:
		EnemyFactory(std::vector<DjipiApp::Enemy>& enemies)
			: m_EnemiesRef(enemies)
		{
			m_RespawnTimer.Start();
			m_TimerModifier = 0;
		}

		void Update(double deltaTime)
		{
			m_TimerModifier += 1;
			int modifier = std::clamp(ENEMY_BASE_SPAWN_TIME - (int)(m_TimerModifier / 300), (int)ENEMY_MIN_SPAWN_TIME, (int)ENEMY_BASE_SPAWN_TIME);

			if (m_RespawnTimer.GetTicks() > modifier)
			{
				m_RespawnTimer.Stop();

				// Spawn enemy
				m_EnemiesRef.emplace_back(Djipi::Vector2(0, 0));

				m_RespawnTimer.Start();
			}
		}
	private :
		std::vector<DjipiApp::Enemy>& m_EnemiesRef;
		Djipi::Timer m_RespawnTimer;

		Uint32 m_TimerModifier;
	};
}