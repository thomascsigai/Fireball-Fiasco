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

		void LooseLife();
		void Reset();

		bool IsGhost() const;
		Uint16 GetLives() const;

	private:
		double m_RotAngle;
		Uint16 m_Lives;

		Djipi::Timer m_GhostTimer;
		bool m_IsGhost;

		// Calculate the player rotation angle to face the mouse position.
		void CalculateRotationAngle();

		// Events
		SDL_Event OnCastFireball = { UserEvents::SPAWN_FIREBALL };
		SDL_Event OnPlayerDie = { UserEvents::PLAYER_DIE };
	};
}