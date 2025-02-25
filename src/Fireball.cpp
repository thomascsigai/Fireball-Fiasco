#include <Fireball.h>

namespace DjipiApp
{
	Fireball::Fireball(Djipi::Vector2 position, Djipi::Vector2 moveDirection)
	{
		m_Transform.SetPosition(position.x, position.y);
		m_Transform.SetSize(FIREBALL_SIZE, FIREBALL_SIZE);

		m_Velocity = moveDirection * FIREBALL_SPEED;
	}

	void Fireball::Update(double deltaTime)
	{
		Djipi::GameObject::Move(deltaTime);
	}
}