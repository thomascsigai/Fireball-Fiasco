#pragma once

#include <SDL.h>
#include <Transform2D.h>
#include <Texture.h>

#include <iostream>

namespace Djipi
{
	class GameObject
	{
	protected:
		std::string m_Name;
		Transform2D m_Transform;
		Vector2 m_Velocity;

		std::weak_ptr<Texture> m_Texture;

	public:
		GameObject(std::shared_ptr<Texture> texture = nullptr);
		GameObject(std::string name, std::shared_ptr<Texture> texture = nullptr);
		GameObject(std::string name, int x, int y, std::shared_ptr<Texture> texture = nullptr);

		virtual void Render(SDL_Renderer* renderer);
		virtual void Update(double deltaTime);
		virtual void Move(double deltaTime);
		virtual void OnCollide(GameObject& other);
		virtual void HandleEvent(SDL_Event& e);

		// Getters
		Transform2D GetTransform2D() const { return m_Transform; }
		std::string GetName() const { return m_Name; }
		SDL_FRect GetCollider() const { return m_Transform.collider; }

		//Setters
		void SetName(std::string name) { m_Name = name; }
		void SetTexture(std::shared_ptr<Texture> texture) { m_Texture = texture; }
	};
}