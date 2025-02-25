#pragma once

#include <Djipi.h>
#include <UserEvents.h>

namespace DjipiApp
{
	class Player : public Djipi::GameObject
	{
	public:
		Player();

		void Update(double deltaTime) override;
		void HandleEvent(SDL_Event& e) override;
		void Render(SDL_Renderer* renderer) override;

	private:
		double m_RotAngle;

		// Calculate the player rotation angle to face the mouse position.
		void CalculateRotationAngle();

		// Events
		SDL_Event OnCastFireball = { UserEvents::SPAWN_FIREBALL };
	};
}