#pragma once
#include "StateMachineBase.h"

namespace bomberman
{
	class RoamingState : public StateMachineBase
	{
	public:
		RoamingState(dae::GameObject& ownerObject);
		~RoamingState() override = default;

		RoamingState(const RoamingState&) = delete;
		RoamingState(RoamingState&&) = delete;
		RoamingState& operator=(const RoamingState&) = delete;
		RoamingState& operator=(RoamingState&&) = delete;

		std::unique_ptr<StateMachineBase> Update(float deltaTime) override;
		void OnEnter() override;
		void OnExit() override;
	};
}

