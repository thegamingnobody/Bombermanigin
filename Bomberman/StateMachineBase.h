#pragma once
#include <GameObject.h>
#include <Event.h>

namespace bomberman
{
	// base class for the states in the state machine
	// not a component
	// owner is stored so the state know who it belongs to
	class StateMachineBase
	{
	public:
		StateMachineBase(dae::GameObject& ownerObject);
		virtual ~StateMachineBase() = default;

		StateMachineBase(const StateMachineBase&) = delete;
		StateMachineBase(StateMachineBase&&) = delete;
		StateMachineBase& operator=(const StateMachineBase&) = delete;
		StateMachineBase& operator=(StateMachineBase&&) = delete;

		virtual std::unique_ptr<StateMachineBase> Update(float deltaTime) = 0;
		virtual void OnEnter() = 0;
		virtual void OnExit() = 0;

		virtual std::unique_ptr<StateMachineBase> Notify(const dae::Event& event) = 0;

	protected:
		dae::GameObject* m_Owner{ nullptr };
	};
}

