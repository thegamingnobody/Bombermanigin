#pragma once
#include "StateMachineBase.h"

namespace bomberman
{
	class PlayerDeathState : public StateMachineBase
	{
	public:
		PlayerDeathState(dae::GameObject& ownerObject, int playerID);
		~PlayerDeathState() override = default;

		PlayerDeathState(const PlayerDeathState& other) = delete;
		PlayerDeathState(PlayerDeathState&& other) = delete;
		PlayerDeathState& operator=(const PlayerDeathState& other) = delete;
		PlayerDeathState& operator=(PlayerDeathState&& other) = delete;

		std::unique_ptr<StateMachineBase> Update(float deltaTime) override;
		void OnEnter() override;
		void OnExit() override;

	private:
		int m_PlayerID;

	};
}

