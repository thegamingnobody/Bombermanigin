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

		PlayerInfo() = default;
	};

	class PlayerManager : public dae::Singleton<PlayerManager>
    {
	public:
		void CreatePlayer(dae::Action::DeviceType deviceType);

		PlayerInfo GetPlayerInfo(int playerID) const
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

