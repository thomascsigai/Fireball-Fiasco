// Defines application entry point.
//

#include <GameConfig.h>
#include <Djipi.h>
#include <UserEvents.h>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <array>

#include <Player.h>
#include <Fireball.h>
#include <Enemy.h>
#include <EnemyFactory.h>

void SpawnFireball(const Djipi::Vector2& playerPosition, const Djipi::Vector2& playerSize,
	std::array<DjipiApp::Fireball, MAX_FIREBALLS_NUMBER>& fireballs, Djipi::ResourceManager& resourceManager)
{
	static size_t index = 0;

	int x, y;
	SDL_GetMouseState(&x, &y);

	Djipi::Vector2 mousePos = Djipi::Vector2(x, y);
	Djipi::Vector2 moveDirection = Djipi::Normalize(mousePos - (playerPosition + (playerSize / 2)));

	Djipi::Vector2 fireballPosition = playerPosition + (playerSize / 2) - Djipi::Vector2(FIREBALL_SIZE / 2, FIREBALL_SIZE / 2);

	fireballs[index] = DjipiApp::Fireball(fireballPosition, moveDirection);
	fireballs[index].SetTexture(resourceManager.GetTexture("resources\\textures\\fireball.png"));
	index = (index + 1) % MAX_FIREBALLS_NUMBER;
}

void RenderGround(Djipi::Renderer& renderer, Djipi::ResourceManager& resourceManager)
{
	const int textureSize = 64;
	int numOfColumns = SCREEN_WIDTH / textureSize + 1;
	int numOfLines = SCREEN_HEIGHT / textureSize + 1;

	std::shared_ptr<Djipi::Texture> texture = resourceManager.GetTexture("resources\\textures\\groundtexture.png");

	if (texture == nullptr)
	{
		APP_LOG_INFO("Could not render ground.");
		return;
	}

	for (int i = 0; i < numOfLines; i++)
	{
		for (int j = 0; j < numOfColumns; j++)
		{
			SDL_Rect rect = {j * textureSize, i * textureSize, textureSize, textureSize };
			SDL_RenderCopy(renderer.GetSDLRenderer(), texture->GetSDLTexture(), NULL, &rect);
		}
	}
}

void RenderOverlay(Djipi::Renderer& renderer, Djipi::ResourceManager& resourceManager)
{
	std::shared_ptr<Djipi::Texture> texture = resourceManager.GetTexture("resources\\textures\\overlay.png");

	if (texture == nullptr)
	{
		APP_LOG_INFO("Could not render ground.");
		return;
	}

	SDL_Rect rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderCopy(renderer.GetSDLRenderer(), texture->GetSDLTexture(), NULL, &rect);
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

	std::array<DjipiApp::Fireball, MAX_FIREBALLS_NUMBER> fireballs;
	std::vector<DjipiApp::Enemy> enemies;

	DjipiApp::EnemyFactory factory = DjipiApp::EnemyFactory(enemies, &resourceManager);

	// MUSIC
	soundManager.PlaySound(resourceManager.GetSound("resources\\sounds\\music.wav"), -1);

	// Events
	SDL_Event OnPlayerHit = { UserEvents::PLAYER_HIT };

	// GAME LOOP
	while (!quit)
	{
		currentTime = SDL_GetTicks();
		deltaTime = (double)(currentTime - previousTime) / 1000;
		previousTime = currentTime;

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
				SpawnFireball(player.GetTransform2D().position, player.GetTransform2D().size, fireballs, resourceManager);
				soundManager.PlaySound(resourceManager.GetSound("resources\\sounds\\fireball.wav"));
			}
			if (e.type == UserEvents::PLAYER_HIT)
			{
				player.LooseLife();
				soundManager.PlaySound(resourceManager.GetSound("resources\\sounds\\lostlive.wav"));
			}
			if (e.type == UserEvents::ENEMY_DIE)
			{
				soundManager.PlaySound(resourceManager.GetSound("resources\\sounds\\ennemideath.wav"));
			}
			if (e.type == UserEvents::FIREBALL_UNSTABLE)
			{
				soundManager.PlaySound(resourceManager.GetSound("resources\\sounds\\cursedFireball.wav"));
			}

			// Handle your events here
			player.HandleEvent(e);
		}

		// COLLISION CHECKING
		
		for (DjipiApp::Fireball& fireball : fireballs)
		{
			if (!fireball.IsUnstable())
			{
				enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
					[&fireball](const DjipiApp::Enemy& enemy) {
						if (Djipi::CheckCollisionAABB(enemy.GetCollider(), fireball.GetCollider()))
						{
							fireball.CollideEnemy();
							return true;
						}
						return false;
					}), enemies.end());
			}
			else
			{
				if (Djipi::CheckCollisionAABB(player.GetCollider(), fireball.GetCollider()) && !player.IsGhost())
				{
					SDL_PushEvent(&OnPlayerHit);
				}
			}
		}

		enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
			[&player, &OnPlayerHit](const DjipiApp::Enemy& enemy) {
				if (Djipi::CheckCollisionAABB(enemy.GetCollider(), player.GetCollider()) && !player.IsGhost())
				{
					SDL_PushEvent(&OnPlayerHit);
					return true;
				}
				return false;
			}), enemies.end());

		// UPDATING
		// Updates methods here

		player.Update(deltaTime);
		factory.Update(deltaTime);

		// RENDERING 
		SDL_SetRenderDrawColor(renderer.GetSDLRenderer(), 240, 240, 240, 255);
		SDL_RenderClear(renderer.GetSDLRenderer());
		SDL_SetRenderDrawColor(renderer.GetSDLRenderer(), 255, 255, 255, 255);

		RenderGround(renderer, resourceManager);
		//RenderOverlay(renderer, resourceManager);
		 
		for (DjipiApp::Fireball& fireball : fireballs)
		{
			fireball.Update(deltaTime);
			fireball.Render(renderer.GetSDLRenderer());
		}

		for (DjipiApp::Enemy& enemy : enemies)
		{
			enemy.UpdateDestination(player.GetTransform2D().position);
			enemy.Update(deltaTime);
			enemy.Render(renderer.GetSDLRenderer());
		}

		// Render all objects in the scene here

		player.Render(renderer.GetSDLRenderer());

		RenderOverlay(renderer, resourceManager);

		SDL_RenderPresent(renderer.GetSDLRenderer());
	}

	SDL_Quit();
	return 0;
}