#pragma once

#include <Djipi.h>

namespace DjipiApp
{
	class Enemy : public Djipi::GameObject
	{
	public:
		Enemy() = default;
		Enemy(Djipi::Vector2 position);

		void Update(double deltaTime) override;
		void Render(SDL_Renderer* renderer) override;
		void UpdateDestination(Djipi::Vector2 destination);

		size_t GetUID() const;

	private:
		size_t m_UID;
		Djipi::Vector2 m_MoveDestination;

		double m_RotAngle;

		void CalculateRotationAngle();
	};
}