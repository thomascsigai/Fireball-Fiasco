#pragma once

#include <SDL.h>

// WINDOW SETTINGS --------------------------------------------------

constexpr Uint16 SCREEN_WIDTH = 800;
constexpr Uint16 SCREEN_HEIGHT = 600;


// RESOURCES SETTINGS ---------------------------------------------------

#define RESOURCES_BASE_DIR "resources"
#define RESOURCES_TEXTURES_DIR "textures"
#define RESOURCES_SOUNDS_DIR "sounds"
#define RESOURCES_FONTS_DIR "fonts"


// GAME SPECIFIC SETTINGS -------------------------------------------

constexpr Uint16 PLAYER_SPEED = 300;

constexpr Uint16 FIREBALL_SPEED = 500;
constexpr Uint16 FIREBALL_SIZE = 10;
const size_t MAX_FIREBALLS_NUMBER = 50;

constexpr Uint16 ENEMY_SIZE = 30;
constexpr Uint16 ENEMY_SPEED = 200;

// KEYBINDS ---------------------------------------------------------

// Paddles keybinds
struct MoveKeybind
{
	SDL_Keycode UP;
	SDL_Keycode DOWN;
	SDL_Keycode LEFT;
	SDL_Keycode RIGHT;
};

constexpr MoveKeybind MOVE_KEYBIND = { SDLK_z, SDLK_s, SDLK_q, SDLK_d };
