#pragma once
#include <GameObject.h>

namespace bomberman
{
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

