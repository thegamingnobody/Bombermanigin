#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	SceneManager::GetInstance().Render();
	SceneManager::GetInstance().RenderImGui();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	auto& camera = Camera::GetInstance();
	glm::vec3 transformedPos = camera.TransformPosition(glm::vec3(x, y, 0));

	SDL_Rect dst{};
	dst.x = static_cast<int>(transformedPos.x);
	dst.y = static_cast<int>(transformedPos.y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	auto& camera = Camera::GetInstance();
	glm::vec3 transformedPos = camera.TransformPosition(glm::vec3(x, y, 0));

	SDL_Rect dst{};
	dst.x = static_cast<int>(transformedPos.x);
	dst.y = static_cast<int>(transformedPos.y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderPoint(float x, float y, const SDL_Color& color) const
{
	auto& camera = Camera::GetInstance();
	glm::vec3 transformedPos = camera.TransformPosition(glm::vec3(x, y, 0));

	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(m_renderer, static_cast<int>(transformedPos.x), static_cast<int>(transformedPos.y));
}

void dae::Renderer::RenderLine(float x1, float y1, float x2, float y2, const SDL_Color& color) const
{
	auto& camera = Camera::GetInstance();
	glm::vec3 transformedPos1 = camera.TransformPosition(glm::vec3(x1, y1, 0));
	glm::vec3 transformedPos2 = camera.TransformPosition(glm::vec3(x2, y2, 0));
	
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(m_renderer, transformedPos1.x, transformedPos1.y, transformedPos2.x, transformedPos2.y);
}

void dae::Renderer::RenderRect(float x, float y, float width, float height, const SDL_Color& color) const
{
	auto& camera = Camera::GetInstance();
	glm::vec3 transformedPos = camera.TransformPosition(glm::vec3(x, y, 0));

	SDL_Rect rect{};
	rect.x = static_cast<int>(transformedPos.x);
	rect.y = static_cast<int>(transformedPos.y);
	rect.w = static_cast<int>(width);
	rect.h = static_cast<int>(height);

	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(m_renderer, &rect);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }

SDL_Window* dae::Renderer::GetSDLWindow() const { return m_window; }