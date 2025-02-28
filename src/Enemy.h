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
			static int counter = 0;
			m_UID = counter++;

			m_Transform.SetPosition(position.x, position.y);
			m_Transform.SetSize(ENEMY_SIZE, ENEMY_SIZE);

			m_MoveDestination = Djipi::Vector2(0, 0);
			m_RotAngle = 0;
		}

		void Update(double deltaTime) override
		{
			Djipi::Vector2 moveDirection = Djipi::Normalize(m_MoveDestination - m_Transform.position);
			m_Velocity = moveDirection * ENEMY_SPEED;

			CalculateRotationAngle();

			GameObject::Move(deltaTime);
		}

		void Render(SDL_Renderer* renderer) override
		{
			if (auto texture = m_Texture.lock())
			{
				SDL_Rect rect = {
			m_Transform.position.x, m_Transform.position.y,
			m_Transform.size.x, m_Transform.size.y
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

		void UpdateDestination(Djipi::Vector2 destination)
		{
			m_MoveDestination = destination;
		}

		size_t GetUID() const {
			return m_UID;
		}

	private:
		size_t m_UID;
		Djipi::Vector2 m_MoveDestination;

		double m_RotAngle;

		void CalculateRotationAngle()
		{
			Djipi::Vector2 centerEnemy = m_Transform.position + (m_Transform.size / 2);

			double cosTheta = (centerEnemy.y - m_MoveDestination.y) / Djipi::Magnitude((m_MoveDestination - centerEnemy));
			m_RotAngle = acos(cosTheta) * 180.0 / M_PI;

			if (m_MoveDestination.x < centerEnemy.x)
			{
				m_RotAngle *= -1;
			}
		}
	};
}