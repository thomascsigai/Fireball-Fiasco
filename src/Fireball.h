#pragma once

#include <Djipi.h>

namespace DjipiApp
{
	class Fireball : public Djipi::GameObject
	{
	public:
		Fireball(Djipi::Vector2 position, Djipi::Vector2 moveDirection)
		{
			m_Transform.SetPosition(position.x, position.y);
			m_Transform.SetSize(10, 10);

			m_Velocity = moveDirection * FIREBALL_SPEED;
		}

		void Update(double deltaTime) override
		{
			Djipi::GameObject::Move(deltaTime);
		}
	};
}