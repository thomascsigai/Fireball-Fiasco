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
constexpr Uint16 PLAYER_SIZE = 30;
constexpr Uint16 GHOST_DURATION = 3000;

constexpr Uint16 FIREBALL_SPEED = 500;
constexpr Uint16 FIREBALL_SIZE = 30;
constexpr Uint16 TIME_BEFORE_UNSTABLE = 3000; // In ms
const size_t MAX_FIREBALLS_NUMBER = 50;

constexpr Uint16 ENEMY_SIZE = 30;
constexpr Uint16 ENEMY_SPEED = 200;
constexpr Uint16 ENEMY_BASE_SPAWN_TIME = 3000;
constexpr Uint16 ENEMY_MIN_SPAWN_TIME = 1000;

// KEYBINDS ---------------------------------------------------------

// Paddles keybinds
struct MoveKeybind
{
	SDL_Keycode UP;
	SDL_Keycode DOWN;
	SDL_Keycode LEFT;
	SDL_Keycode RIGHT;
};

constexpr MoveKeybind MOVE_KEYBIND = { SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT };
