#include <Player.h>
#include <GameConfig.h>

namespace DjipiApp
{
	Player::Player()
	{
		m_Transform.SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		m_Transform.SetSize(30, 20);

		m_RotAngle = 0;
		m_Lives = 3;
	}

	void Player::Update(double deltaTime)
	{
		GameObject::Move(deltaTime);
	}

	void Player::Render(SDL_Renderer* renderer)
	{
		if (auto texture = m_Texture.lock())
		{
			SDL_Rect rect = {
		m_Transform.position.x, m_Transform.position.y,
		m_Transform.size.x, m_Transform.size.y
			};
			SDL_RenderCopyEx(renderer, texture->GetSDLTexture(), NULL, &rect, m_RotAngle, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
		}
		else
		{
			SDL_FRect rect = {
		m_Transform.position.x, m_Transform.position.y,
		m_Transform.size.x, m_Transform.size.y
			};
			SDL_RenderFillRectF(renderer, &rect);
		}

#ifndef NDEBUG
		//Debug draw colliders
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		SDL_RenderDrawRectF(renderer, &m_Transform.collider);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
#endif
	}

	void Player::HandleEvent(SDL_Event& e)
	{
		if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
		{
			if (e.key.keysym.sym == MOVE_KEYBIND.UP) m_Velocity.y -= PLAYER_SPEED;
			if (e.key.keysym.sym == MOVE_KEYBIND.DOWN) m_Velocity.y += PLAYER_SPEED;
			if (e.key.keysym.sym == MOVE_KEYBIND.LEFT) m_Velocity.x -= PLAYER_SPEED;
			if (e.key.keysym.sym == MOVE_KEYBIND.RIGHT) m_Velocity.x += PLAYER_SPEED;
		}
		else if (e.type == SDL_KEYUP && e.key.repeat == 0)
		{
			if (e.key.keysym.sym == MOVE_KEYBIND.UP) m_Velocity.y += PLAYER_SPEED;
			if (e.key.keysym.sym == MOVE_KEYBIND.DOWN) m_Velocity.y -= PLAYER_SPEED;
			if (e.key.keysym.sym == MOVE_KEYBIND.LEFT) m_Velocity.x += PLAYER_SPEED;
			if (e.key.keysym.sym == MOVE_KEYBIND.RIGHT) m_Velocity.x -= PLAYER_SPEED;
		}
		else if (e.type == SDL_MOUSEMOTION)
		{
			CalculateRotationAngle();
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			SDL_PushEvent(&OnCastFireball);
		}
	}

	void Player::CalculateRotationAngle()
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		Djipi::Vector2 mousePos = Djipi::Vector2(x, y);
		Djipi::Vector2 centerPlayer = m_Transform.position + (m_Transform.size / 2);

		double cosTheta = (centerPlayer.y - mousePos.y) / Djipi::Magnitude((mousePos - centerPlayer));
		m_RotAngle = acos(cosTheta) * 180.0 / M_PI;

		if (mousePos.x < centerPlayer.x)
		{
			m_RotAngle *= -1;
		}
	}

	void Player::LooseLife()
	{
		APP_LOG_INFO("Player loose lifes");
		
		if (--m_Lives <= 0)
		{
			APP_LOG_INFO("Player died");
		}
	}
}