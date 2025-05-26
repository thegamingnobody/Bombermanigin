#pragma once
#include "StateMachineBase.h"

namespace bomberman
{
	class PlayerIdleState : public StateMachineBase
	{
	public:
		PlayerIdleState(dae::GameObject& ownerObject, int playerID);
		~PlayerIdleState() override = default;

		PlayerIdleState(const PlayerIdleState& other) = delete;
		PlayerIdleState(PlayerIdleState&& other) = delete;
		PlayerIdleState& operator=(const PlayerIdleState& other) = delete;
		PlayerIdleState& operator=(PlayerIdleState&& other) = delete;

		std::unique_ptr<StateMachineBase> Update(float deltaTime) override;
		void OnEnter() override;
		void OnExit() override;

	private:
		int m_PlayerID;
	};
}

