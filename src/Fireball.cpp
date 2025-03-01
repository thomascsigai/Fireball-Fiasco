#include <Fireball.h>

namespace DjipiApp
{
	Fireball::Fireball(Djipi::Vector2 position, Djipi::Vector2 moveDirection,
		std::shared_ptr<Djipi::Texture> unstableTexture)
	{
		m_Transform.SetPosition(position.x, position.y);
		m_Transform.SetSize(FIREBALL_SIZE, FIREBALL_SIZE);

		m_Velocity = moveDirection * FIREBALL_SPEED;
		m_SavedVelocity = Djipi::Vector2(0, 0);

		m_IsUnstable = false;

		m_UnstableTexture = unstableTexture;
	}

	void Fireball::Update(double deltaTime)
	{
		if (m_UnstableTimer.IsStarted() && m_UnstableTimer.GetTicks() >= TIME_BEFORE_UNSTABLE)
		{
			m_UnstableTimer.Stop();
			BecomeUnstable();
		}

		Djipi::GameObject::Move(deltaTime);
	}

	void Fireball::CollideEnemy()
	{
		if (!m_UnstableTimer.IsStarted())
		{
			m_SavedVelocity = m_Velocity;
			m_Velocity.x = 0;
			m_Velocity.y = 0;

			m_UnstableTimer.Start();
		}
	}

	void Fireball::BecomeUnstable()
	{
		m_IsUnstable = true;
		m_Velocity -= m_SavedVelocity;
		SetTexture(m_UnstableTexture);
		SDL_PushEvent(&OnFireballUnstable);
	}

	bool Fireball::IsUnstable() const
	{
		return m_IsUnstable;
	}
}