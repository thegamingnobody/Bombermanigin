#pragma once
#include <Singleton.h>
#include <Action.h>
#include <vector>
#include <string>
#include <stdexcept>

namespace bomberman
{
	struct PlayerInfo
	{
		int playerID{};
		std::string name{};
		dae::Action::DeviceType deviceType{};
		int score{};
		int lives{ 3 };
		int maxBombs{ 1 };
		std::vector<int> inputIDs{}; // Store all input device IDs for this player

		PlayerInfo() = default;
	};

	struct InputMapping
	{
		dae::Action::DeviceType deviceType{};
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

	class PlayerManager : public dae::Singleton<PlayerManager>
    {
	public:
		void CreatePlayer(dae::Action::DeviceType deviceType, InputMapping inputMapping = InputMapping());

		PlayerInfo& GetPlayerInfo(int playerID)
		{
			if (playerID < 0 || playerID >= static_cast<int>(m_Players.size()))
			{
				throw std::out_of_range("Player ID is out of range.");
			}

			return m_Players[playerID];
		}

	private:
		std::vector<PlayerInfo> m_Players;
    };
}

