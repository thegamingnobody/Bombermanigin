#pragma once
#include "StateMachineBase.h"
#include "EnemyManager.h"
#include "Observer.h"

namespace bomberman
{
	class ChaseState : public StateMachineBase, public dae::Observer
	{
	public:
		ChaseState(dae::GameObject& ownerObject);
		~ChaseState() override = default;

		ChaseState(const ChaseState&) = delete;
		ChaseState(ChaseState&&) = delete;
		ChaseState& operator=(const ChaseState&) = delete;
		ChaseState& operator=(ChaseState&&) = delete;

		std::unique_ptr<StateMachineBase> Update(float deltaTime) override;
		void OnEnter() override;
		void OnExit() override;

		void Notify(const dae::Event& event) override;

	private:
		bool IsDirectionValid(glm::vec3 direction) const;

		bool m_HasDied{ false };
		bomberman::EnemyData m_EnemyData;
	};
}


