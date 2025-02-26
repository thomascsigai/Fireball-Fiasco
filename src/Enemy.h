#pragma once

#include <Djipi.h>

namespace DjipiApp
{
	class Enemy : public Djipi::GameObject
	{
	public:
		Enemy() = default;

		Enemy(Djipi::Vector2 position)
		{
			m_Transform.SetPosition(position.x, position.y);
			m_Transform.SetSize(ENEMY_SIZE, ENEMY_SIZE);

			m_MoveDestination = Djipi::Vector2(0, 0);
		}

		void Update(double deltaTime) override
		{
			Djipi::Vector2 moveDirection = Djipi::Normalize(m_MoveDestination - m_Transform.position);
			m_Velocity = moveDirection * ENEMY_SPEED;

			GameObject::Move(deltaTime);
		}

		void UpdateDestination(Djipi::Vector2 destination)
		{
			m_MoveDestination = destination;
		}

	private:
		Djipi::Vector2 m_MoveDestination;
	};
}