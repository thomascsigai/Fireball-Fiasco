#include <Player.h>
#include <GameConfig.h>

namespace DjipiApp
{
	Player::Player()
	{
		m_Transform.SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		m_Transform.SetSize(30, 20);

		m_RotAngle = 0;
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
			APP_LOG_INFO("Shoot");
		}
	}

	/*
	* Calculate the player rotation angle to face the mouse position.
	*/
	void Player::CalculateRotationAngle()
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		Djipi::Vector2 mousePos = Djipi::Vector2(x, y);

		double cosTheta = (m_Transform.position.y - mousePos.y) / Djipi::Magnitude((mousePos - m_Transform.position));
		m_RotAngle = acos(cosTheta) * 180.0 / M_PI;

		if (mousePos.x < m_Transform.position.x)
		{
			m_RotAngle *= -1;
		}
	}
}