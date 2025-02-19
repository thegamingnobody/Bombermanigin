#include "TextureComponent.h"
#include "ResourceManager.h"
#include <iostream>
#include "Renderer.h"
#include "GameObject.h"

void dae::TextureComponent::Render() const
{
	if (m_pTextures[m_CurrentSpriteIndex] == nullptr)
	{
		throw std::invalid_argument("Texture does not exist");
	}

	auto position = GetOwner()->GetGlobalPosition();
	dae::Renderer::GetInstance().RenderTexture(*m_pTextures[m_CurrentSpriteIndex], position.x, position.y);
}

int dae::TextureComponent::AddTexture(const std::string& filename)
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

	return static_cast<int>(m_pTextures.size()) - 1;
}

int dae::TextureComponent::AddTexture(const std::shared_ptr<Texture2D>& texture)
{
	if (texture == nullptr)
	{
		throw std::invalid_argument("Texture doesn't exist");
	}

	m_pTextures.emplace_back(texture);

	return static_cast<int>(m_pTextures.size()) - 1;
}

void dae::TextureComponent::SetCurrentIndex(int const newIndex)
{
	if (newIndex < static_cast<int>(m_pTextures.size()) and newIndex >= 0)
	{
		m_CurrentSpriteIndex = newIndex;
	}
	else
	{
		throw std::out_of_range("New Index out of range");
	}
}

dae::TextureComponent::TextureComponent(dae::GameObject* object)
	: RenderComponent(object)
	, m_CurrentSpriteIndex(0)
{
}