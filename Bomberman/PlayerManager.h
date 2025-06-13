#pragma once
#include <Singleton.h>
#include <Action.h>
#include <vector>
#include <string>
#include <stdexcept>
#include "GameObject.h"
#include <memory>

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
		int lives{ 0 };
		int maxBombs{ 1 };
		std::vector<int> inputIDs{2}; // Store all input device IDs for this player
		std::vector<bomberman::InputMapping> inputMappings{2};

		PlayerInfo() = default;
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

		void RemovePlayer(int playerID);
		void ClearPlayers();

		void AddScore(int scoreToAdd);
		void ResetScore();

		int GetScore() const { return m_Score; }

	private:
		PlayerInfo CreatePlayerInfo(InputMapping mapping1, InputMapping mapping2 = InputMapping());
		std::shared_ptr<dae::GameObject> CreatePlayerObject(const PlayerInfo& playerInfo);
		void CreatePlayerActions(dae::GameObject& playerObject, PlayerInfo& playerInfo);


		std::vector<PlayerInfo> m_Players;
		int m_Score{ 0 };
	};
}

