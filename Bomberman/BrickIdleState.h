#pragma once
#include "StateMachineBase.h"
#include <GameObject.h>

namespace bomberman
{
	//Giving the bricks their own state machine is maybe a bit redundant,
	//but it makes everything more uniform (and a destruction animation would be easier this way)
	class BrickIdleState : public StateMachineBase
	{
	public:
		BrickIdleState(dae::GameObject& ownerObject);
		~BrickIdleState() override = default;
	
		void OnEnter() override;
		std::unique_ptr<StateMachineBase> Update(float deltaTime) override;
		void OnExit() override;
	};
}
