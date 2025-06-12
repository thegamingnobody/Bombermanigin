#pragma once
#include <Singleton.h>
#include <vector>
#include "LevelData.h"

namespace bomberman
{
	enum class GameMode
	{
		Selecting,
		Singleplayer,
		Coop,
		Versus
	};

	class GameManager : public dae::Singleton<GameManager>
	{
	public:
		void Init();

		LevelData GetLevelData() const { return m_CurrentLevelData; }

		void LoadNextLevel();

		void ResetLevel();

		GameMode GetGameMode() const { return m_GameMode; }
		void SetGameMode(GameMode mode) { m_GameMode = mode; }

		void ResetLevelCount();
	private:
		void LoadLevel(int level); 

		int const m_MaxLevelAmount{ 3 };

		// This is the data to start a level, not the current state.
		// Current map state is handled by the "Grid" class.
		LevelData m_CurrentLevelData{};

		GameMode m_GameMode{ GameMode::Selecting };
	};

}