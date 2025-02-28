#pragma once

#include <SDL.h>

namespace UserEvents
{
	// Place here your events you have to use in your app
	const inline Uint32 SPAWN_FIREBALL = SDL_RegisterEvents(1);
	const inline Uint32 PLAYER_HIT = SDL_RegisterEvents(1);
	const inline Uint32 PLAYER_DIE = SDL_RegisterEvents(1);
	const inline Uint32 ENEMY_DIE = SDL_RegisterEvents(1);
	const inline Uint32 FIREBALL_UNSTABLE = SDL_RegisterEvents(1);
}