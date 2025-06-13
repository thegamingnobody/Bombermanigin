#include "NameEntryManager.h"
#include "SceneManager.h"
#include "SceneNames.h"
#include <Scene.h>
#include <TextComponent.h>
#include <GameObject.h>
#include "StateMachineComponent.h"
#include "NameConfirmEvent.h"

void bomberman::NameEntryManager::Init()
{
	m_Name = { 'A', 'A', 'A' };
	m_CurrentIndex = 0;
}

void bomberman::NameEntryManager::Reset()
{
	m_Name = { 'A', 'A', 'A' };

	for (int i = 0; i < m_MaxIndex; i++)
	{
		m_CurrentIndex = i;
		UpdateLetterDisplay();
	}
	m_CurrentIndex = 0;
}

void bomberman::NameEntryManager::MoveRight()
{
	m_CurrentIndex++;
	if (m_CurrentIndex > m_MaxIndex)
	{
		m_CurrentIndex = 0;
	}
	
	UpdateLetterDisplay();
}

void bomberman::NameEntryManager::MoveLeft()
{
	m_CurrentIndex--;
	if (m_CurrentIndex < 0)
	{
		m_CurrentIndex = m_MaxIndex;
	}

	UpdateLetterDisplay();
}

void bomberman::NameEntryManager::IncrementLetter()
{
	char& currentChar = m_Name[m_CurrentIndex];
	if (currentChar < 'Z')
	{
		currentChar++;
	}
	else
	{
		currentChar = 'A';
	}

	UpdateLetterDisplay();
}

void bomberman::NameEntryManager::DecrementLetter()
{
	char& currentChar = m_Name[m_CurrentIndex];
	if (currentChar > 'A')
	{
		currentChar--;
	}
	else
	{
		currentChar = 'Z';
	}

	UpdateLetterDisplay();
}

void bomberman::NameEntryManager::ConfirmName()
{
	auto gameStateScene = dae::SceneManager::GetInstance().GetScene(SCENE_GAME_STATE);
	auto gameStateObject = gameStateScene->GetObject("GameState");
	auto gameStateComponent = gameStateObject->GetComponent<bomberman::StateMachineComponent>();

	if (gameStateComponent.has_value())
	{
		NameConfirmEvent event{ GetName() };
		gameStateComponent.value()->Notify(event);
	}
}

void bomberman::NameEntryManager::UpdateLetterDisplay() const
{
	auto gameOverScene = dae::SceneManager::GetInstance().GetScene(SCENE_GAMEOVER);
	auto selectedLetter = gameOverScene->GetObject("Letter"+std::to_string(m_CurrentIndex));
	auto textComponent = selectedLetter->GetComponent<dae::TextComponent>();

	if (textComponent.has_value())
	{
		textComponent.value()->SetText(std::string(1, m_Name[m_CurrentIndex]));
	}

	auto cursor = gameOverScene->GetObject("Cursor");
	textComponent = cursor->GetComponent<dae::TextComponent>();

	if (textComponent.has_value())
	{
		std::string cursorText = "   ";
		cursorText[m_CurrentIndex] = 'V';
		textComponent.value()->SetText(cursorText);
	}
}

std::string bomberman::NameEntryManager::GetName() const
{
	return std::string(m_Name.data(), m_Name.size());
}