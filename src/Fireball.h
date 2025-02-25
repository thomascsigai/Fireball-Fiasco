#pragma once

#include <Djipi.h>

namespace DjipiApp
{
	class Fireball : public Djipi::GameObject
	{
	public:
		Fireball() = default;
		Fireball(Djipi::Vector2 position, Djipi::Vector2 moveDirection);

		void Update(double deltaTime) override;
	};
}