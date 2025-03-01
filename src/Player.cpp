#include <Player.h>
#include <GameConfig.h>

namespace DjipiApp
{
	Player::Player()
	{
		m_Transform.SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		m_Transform.SetSize(PLAYER_SIZE, PLAYER_SIZE);

		m_RotAngle = 0;
		m_Lives = 3;
		m_IsGhost = false;
	}

	void Player::Update(double deltaTime)
	{
		if (m_IsGhost)
		{
			if (m_GhostTimer.GetTicks() > GHOST_DURATION)
			{
				m_IsGhost = false;
			}
		}

		if (m_Transform.position.y <= 0)
		{
			m_Transform.SetPosition(m_Transform.position.x, 0);
		}
		
		if (m_Transform.position.y >= SCREEN_HEIGHT - PLAYER_SIZE)
		{
			m_Transform.SetPosition(m_Transform.position.x, SCREEN_HEIGHT - PLAYER_SIZE);
		}
		
		if (m_Transform.position.x <= 0)
		{
			m_Transform.SetPosition(0, m_Transform.position.y);
		}

		if (m_Transform.position.x >= SCREEN_WIDTH - PLAYER_SIZE)
		{
			m_Transform.SetPosition(SCREEN_WIDTH - PLAYER_SIZE, m_Transform.position.y);
		}

		GameObject::Move(deltaTime);
	}

	void Player::Render(SDL_Renderer* renderer)
	{
		auto texture = m_Texture.lock();
		if (texture && !m_IsGhost)
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
			if (!m_IsGhost)
			{
				SDL_PushEvent(&OnCastFireball);
			}
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
		if (m_GhostTimer.IsStarted() && m_GhostTimer.GetTicks() < GHOST_DURATION)
		{
			APP_LOG_INFO("Player is ghost");
			return;
		}
		m_GhostTimer.Stop();
		m_GhostTimer.Start();
		
		m_IsGhost = true;
		
		if (--m_Lives <= 0)
		{
			SDL_PushEvent(&OnPlayerDie);
		}
	}

	void Player::Reset()
	{
		m_Transform.SetPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		m_RotAngle = 0;
		m_Lives = 3;
		m_IsGhost = false;

		m_Velocity.x = 0;
		m_Velocity.y = 0;
	}

	bool Player::IsGhost() const
	{
		return m_IsGhost;
	}

	Uint16 Player::GetLives() const
	{
		return m_Lives;
	}
}