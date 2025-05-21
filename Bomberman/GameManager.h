#pragma once
#include <Singleton.h>
#include <vector>
#include "LevelData.h"

namespace bomberman
{
	class GameManager : public dae::Singleton<GameManager>
	{
	public:
		void Init();

		LevelData GetLevelData() const { return m_CurrentLevelData; }

		void LoadNextLevel();

		void ResetLevel();

	private:
		void LoadLevel(int level); 

		int const m_MaxLevelAmount{ 3 };

		// This is the data to start a level, not the current state.
		// Current map state is handled by the "Grid" class.
		LevelData m_CurrentLevelData{};

	};

}