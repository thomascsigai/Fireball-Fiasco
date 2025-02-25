#pragma once

#include <Djipi.h>

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

		void CalculateRotationAngle();
	};
}