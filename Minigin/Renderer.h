#pragma once
#include <SDL.h>
#include "Singleton.h"
#include "Camera.h"

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		void RenderPoint(float x, float y, const SDL_Color& color = SDL_Color(255, 0, 0, 255)) const;
		void RenderLine(float x1, float y1, float x2, float y2, const SDL_Color& color = SDL_Color(255, 0, 0, 255)) const;
		void RenderRect(float x, float y, float width, float height, const SDL_Color& color = SDL_Color(255, 0, 0, 255)) const;

		SDL_Renderer* GetSDLRenderer() const;
		SDL_Window* GetSDLWindow() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

