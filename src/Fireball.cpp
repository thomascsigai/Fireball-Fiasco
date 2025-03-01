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
		m_RotAngle = 0;
	}


	void Fireball::Render(SDL_Renderer* renderer)
	{
		auto texture = m_Texture.lock();
		if (texture)
		{
			SDL_Rect rect = {
		m_Transform.position.x - 5, m_Transform.position.y - 5,
		m_Transform.size.x + 10, m_Transform.size.y + 10
			};
			SDL_RenderCopyEx(renderer, texture->GetSDLTexture(), NULL, &rect, m_RotAngle, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
		}
		else
		{
			SDL_FRect rect = {
		m_Transform.position.x, m_Transform.position.y,
		m_Transform.size.x, m_Transform.size.y
			};
			SDL_RenderFillRectF(renderer, &rect);
		}

#ifndef NDEBUG
		//Debug draw colliders
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawRectF(renderer, &m_Transform.collider);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
#endif
	}

	void Fireball::Update(double deltaTime)
	{
		if (m_UnstableTimer.IsStarted() && m_UnstableTimer.GetTicks() >= TIME_BEFORE_UNSTABLE)
		{
			m_UnstableTimer.Stop();
			BecomeUnstable();
		}

		if (m_Transform.position.y <= 0 || m_Transform.position.y >= SCREEN_HEIGHT - FIREBALL_SIZE
			|| m_Transform.position.x <= 0 || m_Transform.position.x >= SCREEN_WIDTH - FIREBALL_SIZE)
		{
			BecomeUnstable();
		}

		CalculateRotationAngle();

		Djipi::GameObject::Move(deltaTime);
	}

	void Fireball::CalculateRotationAngle()
	{
		if (m_Velocity.x == 0 && m_Velocity.y == 0) return;

		float angle = std::atan2(m_Velocity.y, m_Velocity.x);
		m_RotAngle = angle * (180.0f / M_PI);
		m_RotAngle += 45;
	}

	void Fireball::CollideEnemy()
	{
		if (!m_UnstableTimer.IsStarted() && !m_IsUnstable)
		{
			m_SavedVelocity = m_Velocity;
			m_Velocity.x = 0;
			m_Velocity.y = 0;

			m_UnstableTimer.Start();
		}
	}

	void Fireball::BecomeUnstable()
	{
		if (!m_IsUnstable)
		{
			m_IsUnstable = true;

			if (m_Velocity.x != 0 && m_Velocity.y != 0)
			{
				m_Velocity = -m_Velocity;
			}
			else
			{
				m_Velocity -= m_SavedVelocity;
			}

			SetTexture(m_UnstableTexture);
			SDL_PushEvent(&OnFireballUnstable);
		}
	}

	bool Fireball::IsUnstable() const
	{
		return m_IsUnstable;
	}
}