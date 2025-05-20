#pragma once
#include <GameObject.h>

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

		virtual std::unique_ptr<StateMachineBase> Update(float /*deltaTime*/) { return nullptr; }
		virtual void OnEnter() {}
		virtual void OnExit() {}

	protected:
		dae::GameObject* m_Owner{ nullptr };
	};
}

