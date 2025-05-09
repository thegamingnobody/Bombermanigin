#pragma once
#include <Component.h>
#include  "StateMachineBase.h"
#include <map>

namespace bomberman
{
    class StateMachineComponent : public dae::Component
    {
	public:
		StateMachineComponent(dae::GameObject& ownerObject, std::unique_ptr<StateMachineBase> startingState);
		~StateMachineComponent() override = default;

		StateMachineComponent(const StateMachineComponent&) = delete;
		StateMachineComponent(StateMachineComponent&&) = delete;
		StateMachineComponent& operator=(const StateMachineComponent&) = delete;
		StateMachineComponent& operator=(StateMachineComponent&&) = delete;

		void Update(float deltaTime) override;
		//void Render() const override;
		//void RenderImGui() override;

		void ChangeState(std::unique_ptr<StateMachineBase> newState);

	private:
		std::unique_ptr<StateMachineBase> m_CurrentState;
	};
}


