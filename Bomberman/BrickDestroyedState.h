#pragma once
#include "StateMachineBase.h"
#include <Event.h>

namespace bomberman
{
	//Giving the bricks their own state machine is maybe a bit redundant,
	//but it makes everything more uniform (and a destruction animation would be easier this way)
	class BrickDestroyedState : public StateMachineBase
	{
	public:
		BrickDestroyedState(dae::GameObject& ownerObject);
		~BrickDestroyedState() override = default;
		
		std::unique_ptr<StateMachineBase> Update(float deltaTime) override;
		void OnEnter() override;
		void OnExit() override;

		std::unique_ptr<StateMachineBase> Notify(const dae::Event& event) override;
	};
}

