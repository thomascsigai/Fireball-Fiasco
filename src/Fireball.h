#pragma once

#include <Djipi.h>

namespace DjipiApp
{
	class Fireball : public Djipi::GameObject
	{
	public:
		Fireball() = default;
		Fireball(Djipi::Vector2 position, Djipi::Vector2 moveDirection);

		void Update(double deltaTime) override;

		void CollideEnemy()
		{
			m_SavedVelocity = m_Velocity;
			m_Velocity.x = 0;
			m_Velocity.y = 0;
		}

	private:
		Djipi::Vector2 m_SavedVelocity;
	};
}