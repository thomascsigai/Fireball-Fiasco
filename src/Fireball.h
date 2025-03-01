#pragma once

#include <Djipi.h>
#include <UserEvents.h>

namespace DjipiApp
{
	class Fireball : public Djipi::GameObject
	{
	public:
		Fireball() = default;
		Fireball(Djipi::Vector2 position, Djipi::Vector2 moveDirection,
			std::shared_ptr<Djipi::Texture> unstableTexture);

		void Update(double deltaTime) override;
		void Render(SDL_Renderer* renderer) override;

		void CollideEnemy();
		bool IsUnstable() const;

	private:
		Djipi::Vector2 m_SavedVelocity;
		Djipi::Timer m_UnstableTimer;

		bool m_IsUnstable;
		double m_RotAngle;

		void BecomeUnstable();
		void CalculateRotationAngle();

		std::shared_ptr<Djipi::Texture> m_UnstableTexture;

		SDL_Event OnFireballUnstable = { UserEvents::FIREBALL_UNSTABLE };
	};
}