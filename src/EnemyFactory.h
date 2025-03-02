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
		EnemyFactory(std::vector<DjipiApp::Enemy>& enemies, Djipi::ResourceManager* resourceManager = nullptr);

		void Start();
		void Reset();
		void Update(double deltaTime);

	private :
		std::vector<DjipiApp::Enemy>& m_EnemiesRef;
		Djipi::Timer m_RespawnTimer;

		Uint32 m_TimerModifier;
		std::mt19937 m_Gen;

		Djipi::ResourceManager* ptr_ResourceManager;

		void SpawnEnemy();
	};
}