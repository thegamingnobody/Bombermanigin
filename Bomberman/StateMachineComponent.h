#pragma once
#include <Component.h>
#include  "StateMachineBase.h"
#include <map>
#include "EnemyManager.h"

namespace bomberman
{
    class StateMachineComponent : public dae::Component
    {
	public:
		//Don't give a state to the contructor, 
		//because if the statemachinecomponent is needed in the contructor or on enter of the state,
		//it won't be added to the object yet
		//StateMachineComponent(dae::GameObject& ownerObject, std::unique_ptr<StateMachineBase> startingState);
		StateMachineComponent(dae::GameObject& ownerObject);
		virtual ~StateMachineComponent() override;

		StateMachineComponent(const StateMachineComponent&) = delete;
		StateMachineComponent(StateMachineComponent&&) = delete;
		StateMachineComponent& operator=(const StateMachineComponent&) = delete;
		StateMachineComponent& operator=(StateMachineComponent&&) = delete;

		void Update(float deltaTime) override;
		//void Render() const override;
		//void RenderImGui() override;

		void ChangeState(std::unique_ptr<StateMachineBase> newState);

		StateMachineBase* GetCurrentState() const { return m_CurrentState.get(); }

		bomberman::EnemyData GetEnemyData() const { return m_EnemyData; }

	private:
		void LoadEnemyData();

		std::unique_ptr<StateMachineBase> m_CurrentState;
		bomberman::EnemyData m_EnemyData;
	};
}


