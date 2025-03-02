#include <Enemy.h>

namespace DjipiApp
{
	Enemy::Enemy(Djipi::Vector2 position)
	{
		static int counter = 0;
		m_UID = counter++;

		m_Transform.SetPosition(position.x, position.y);
		m_Transform.SetSize(ENEMY_SIZE, ENEMY_SIZE);

		m_MoveDestination = Djipi::Vector2(0, 0);
		m_RotAngle = 0;
	}

	void Enemy::Update(double deltaTime)
	{
		Djipi::Vector2 moveDirection = Djipi::Normalize(m_MoveDestination - m_Transform.position);
		m_Velocity = moveDirection * ENEMY_SPEED;

		CalculateRotationAngle();

		GameObject::Move(deltaTime);
	}

	void Enemy::Render(SDL_Renderer* renderer)
	{
		if (auto texture = m_Texture.lock())
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

	void Enemy::UpdateDestination(Djipi::Vector2 destination)
	{
		m_MoveDestination = destination;
	}

	size_t Enemy::GetUID() const
	{
		return m_UID;
	}

	void Enemy::CalculateRotationAngle()
	{
		Djipi::Vector2 centerEnemy = m_Transform.position + (m_Transform.size / 2);

		double cosTheta = (centerEnemy.y - m_MoveDestination.y) / Djipi::Magnitude((m_MoveDestination - centerEnemy));
		m_RotAngle = acos(cosTheta) * 180.0 / M_PI;

		if (m_MoveDestination.x < centerEnemy.x)
		{
			m_RotAngle *= -1;
		}
	}
}