// Defines application entry point.
//

#include <GameConfig.h>
#include <Djipi.h>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <vector>

#include <Player.h>
#include <Fireball.h>
#include <UserEvents.h>

void SpawnFireball(const Djipi::Vector2& playerPosition, std::vector<DjipiApp::Fireball>& fireballs)
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	Djipi::Vector2 mousePos = Djipi::Vector2(x, y);
	Djipi::Vector2 moveDirection = Djipi::Normalize(mousePos - playerPosition);

	fireballs.emplace_back(playerPosition, moveDirection);
}

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		LOG_FATAL("SDL could not initialize ! SDL_Error: " << SDL_GetError());
		return 0;
	}

	bool quit = false;

	SDL_Event e;

	Djipi::Window window = Djipi::Window();
	Djipi::Renderer renderer = Djipi::Renderer(&window);
	Djipi::SoundManager soundManager = Djipi::SoundManager();

	Djipi::ResourceManager resourceManager = Djipi::ResourceManager(&renderer);

	Uint64 previousTime = SDL_GetTicks();
	Uint64 currentTime;
	double deltaTime;

	// GAMEOBJECTS
	// Create your gameobjects here
	
	DjipiApp::Player player = DjipiApp::Player();
	player.SetTexture(resourceManager.GetTexture("resources\\textures\\player.png"));

	std::vector<DjipiApp::Fireball> fireballs = std::vector<DjipiApp::Fireball>();

	// GAME LOOP
	while (!quit)
	{
		// EVENTS LOOP
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
			{
				if (e.key.keysym.sym == SDLK_SPACE)
					resourceManager.ReloadAllResources();
			}
			if (e.type == UserEvents::SPAWN_FIREBALL)
			{
				SpawnFireball(player.GetTransform2D().position, fireballs);
			}

			// Handle your events here
			player.HandleEvent(e);
		}

		currentTime = SDL_GetTicks();
		deltaTime = (double)(currentTime - previousTime) / 1000;
		previousTime = currentTime;

		// UPDATING
		// Updates methods here

		player.Update(deltaTime);

		// RENDERING 
		SDL_SetRenderDrawColor(renderer.GetSDLRenderer(), 0, 0, 0, 255);
		SDL_RenderClear(renderer.GetSDLRenderer());
		SDL_SetRenderDrawColor(renderer.GetSDLRenderer(), 255, 255, 255, 255);

		for (DjipiApp::Fireball& fireball : fireballs)
		{
			fireball.Update(deltaTime);
			fireball.Render(renderer.GetSDLRenderer());
		}

		// Render all objects in the scene here

		player.Render(renderer.GetSDLRenderer());

		SDL_RenderPresent(renderer.GetSDLRenderer());
	}

	SDL_Quit();
	return 0;
}