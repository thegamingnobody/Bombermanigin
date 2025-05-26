#pragma once
#include "StateMachineBase.h"

namespace bomberman
{
	class DyingState : public StateMachineBase
	{
	public:
		DyingState(dae::GameObject& ownerObject);
		~DyingState() override = default;

		DyingState(const DyingState&) = delete;
		DyingState(DyingState&&) = delete;
		DyingState& operator=(const DyingState&) = delete;
		DyingState& operator=(DyingState&&) = delete;

		std::unique_ptr<StateMachineBase> Update(float deltaTime) override;
		void OnEnter() override;
		void OnExit() override;
	};

};