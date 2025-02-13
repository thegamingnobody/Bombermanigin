#include "TextureComponent.h"
#include "ResourceManager.h"
#include <iostream>
#include "Renderer.h"

void dae::TextureComponent::Render() const
{
	if (m_pTextures[m_CurrentSpriteIndex] == nullptr)
	{
		throw std::invalid_argument("Texture does not exist");
	}

	dae::Renderer::GetInstance().RenderTexture(*m_pTextures[m_CurrentSpriteIndex], 0.0f, 0.0f);
	//todo: add ability to render at location in transformcomponent
}

void dae::TextureComponent::AddTexture(const std::string& filename)
{
	try
	{
		auto newTexture{ dae::ResourceManager::GetInstance().LoadTexture(filename) };
		m_pTextures.emplace_back(newTexture);
		newTexture.reset();
	}
	catch (...)
	{
		std::cout << "Invalid path: \"" << filename << "\"\n";
	}
}

void dae::TextureComponent::SetCurrentIndex(int const newIndex)
{
	if (newIndex < m_pTextures.size() and newIndex >= 0)
	{
		m_CurrentSpriteIndex = newIndex;
	}
	else
	{
		throw std::out_of_range("New Index out of range");
	}
}
