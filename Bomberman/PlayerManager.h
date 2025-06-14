#pragma once
#include <Singleton.h>
#include <Action.h>
#include <vector>
#include <string>
#include <stdexcept>
#include "GameObject.h"
#include <memory>
#include <algorithm>

namespace bomberman
{

	struct InputMapping
	{
		dae::Action::DeviceType deviceType{ dae::Action::DeviceType::UnUsed };
		int upButton{};
		int downButton{};
		int leftButton{};
		int rightButton{};
		int bombButton{};

		InputMapping()
			: deviceType(dae::Action::DeviceType::Keyboard)
			, upButton(static_cast<int>(dae::KeyboardKeys::W))
			, downButton(static_cast<int>(dae::KeyboardKeys::S))
			, leftButton(static_cast<int>(dae::KeyboardKeys::A))
			, rightButton(static_cast<int>(dae::KeyboardKeys::D))
			, bombButton(static_cast<int>(dae::KeyboardKeys::C))
		{
		}

		InputMapping(dae::Action::DeviceType type, int up, int down, int left, int right, int bomb)
			: deviceType(type)
			, upButton(up)
			, downButton(down)
			, leftButton(left)
			, rightButton(right)
			, bombButton(bomb) 
		{
		}

		InputMapping(dae::KeyboardKeys up, dae::KeyboardKeys down, dae::KeyboardKeys left, dae::KeyboardKeys right, dae::KeyboardKeys bomb)
			: deviceType(dae::Action::DeviceType::Keyboard)
			, upButton(static_cast<int>(up))
			, downButton(static_cast<int>(down))
			, leftButton(static_cast<int>(left))
			, rightButton(static_cast<int>(right))
			, bombButton(static_cast<int>(bomb)) 
		{
		}

		InputMapping(dae::GamepadButtons up, dae::GamepadButtons down, dae::GamepadButtons left, dae::GamepadButtons right, dae::GamepadButtons bomb)
			: deviceType(dae::Action::DeviceType::Gamepad)
			, upButton(static_cast<int>(up))
			, downButton(static_cast<int>(down))
			, leftButton(static_cast<int>(left))
			, rightButton(static_cast<int>(right))
			, bombButton(static_cast<int>(bomb)) 
		{
		}
	};

	struct PlayerInfo
	{
		int playerID{};
		std::string name{};
		int maxBombs{ 1 };
		std::vector<int> inputIDs{2}; // Store all input device IDs for this player
		std::vector<bomberman::InputMapping> inputMappings{2};
		bool isAlive{ true };

		PlayerInfo() = default;

		int GetBombRange() const { return bombRange; }
		void SetBombRange(int range) 
		{
			bombRange = range;
			if (bombRange < 1) 
			{
				bombRange = 1;
			}
			if (bombRange > maxBombRange)
			{
				bombRange = maxBombRange;
			}
		}
		void IncreaseBombRange() 
		{
			SetBombRange(bombRange + 1);
		}
	private:
		int bombRange{ 1 };
		int maxBombRange{ 3 };
	};

	class PlayerManager : public dae::Singleton<PlayerManager>
    {
	public:
		void CreatePlayer(InputMapping mapping1, InputMapping mapping2 = InputMapping());

		PlayerInfo& GetPlayerInfo(int playerID)
		{
			if (playerID < 0 || playerID >= static_cast<int>(m_Players.size()))
			{
				throw std::out_of_range("Player ID is out of range.");
			}

			return m_Players[playerID];
		}
		PlayerInfo& GetPlayerInfo(const std::string& playerName)
		{
			auto it = std::find_if(m_Players.begin(), m_Players.end(), [&playerName](const PlayerInfo& player) { return player.name == playerName; });

			if (it != m_Players.end())
			{
				return *it;
			}
			throw std::runtime_error("Player not found: " + playerName);
		}

		void RemovePlayer(int playerID);
		void ClearPlayers();

		void AddScore(int scoreToAdd);
		void ResetScore();

		int GetScore() const { return m_Score; }
		
		// Returns false if no players left, returns true if players left
		bool SetPlayerDied(int playerID);

		void ResetPlayersLifeState();

		bool AreAllPlayersDead() const
		{
			auto aliveCount = std::count_if(m_Players.begin(), m_Players.end(), [](const PlayerInfo& player) { return player.isAlive; });
			
			return (aliveCount == 0);
		}

		int GetLives() const { return m_Lives; }
		void DecreaseLives()
		{
			m_Lives--;
		}

	private:
		PlayerInfo CreatePlayerInfo(InputMapping mapping1, InputMapping mapping2 = InputMapping());
		std::shared_ptr<dae::GameObject> CreatePlayerObject(const PlayerInfo& playerInfo);
		void CreatePlayerActions(dae::GameObject& playerObject, PlayerInfo& playerInfo);

		std::vector<PlayerInfo> m_Players;
		int m_Score{ 0 };
		int m_Lives{ 3 };
		int const m_MaxLives{ 3 };
	};
}

