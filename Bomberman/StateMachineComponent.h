#pragma once
#include <Component.h>
#include "StateMachineBase.h"
#include <map>
#include "EnemyManager.h"
#include <Observer.h>

namespace bomberman
{
	// the actual state machine itself that keeps track of the current state
	class StateMachineComponent : public dae::Component, public dae::Observer
    {
	public:
		//Don't give a state to the contructor, 
		//because if the statemachinecomponent is needed in the contructor or on enter of the state,
		//it won't be added to the object yet
		StateMachineComponent(dae::GameObject& ownerObject);
		//StateMachineComponent(dae::GameObject& ownerObject, std::unique_ptr<StateMachineBase> startingState);
		virtual ~StateMachineComponent() override;

		StateMachineComponent(const StateMachineComponent&) = delete;
		StateMachineComponent(StateMachineComponent&&) = delete;
		StateMachineComponent& operator=(const StateMachineComponent&) = delete;
		StateMachineComponent& operator=(StateMachineComponent&&) = delete;

		void Update(float deltaTime) override;

		void ChangeState(std::unique_ptr<StateMachineBase> newState);

		StateMachineBase* GetCurrentState() const { return m_CurrentState.get(); }

		void Notify(const dae::Event& event) override;

	private:
		std::unique_ptr<StateMachineBase> m_CurrentState;
	};
}