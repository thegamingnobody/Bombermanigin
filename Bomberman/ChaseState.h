#pragma once
#include "StateMachineBase.h"
#include "EnemyManager.h"
#include "Observer.h"

namespace bomberman
{
	class ChaseState : public StateMachineBase
	{
	public:
		ChaseState(dae::GameObject& ownerObject, const bomberman::EnemyData& enemyData);
		~ChaseState() override = default;

		ChaseState(const ChaseState&) = delete;
		ChaseState(ChaseState&&) = delete;
		ChaseState& operator=(const ChaseState&) = delete;
		ChaseState& operator=(ChaseState&&) = delete;

		std::unique_ptr<StateMachineBase> Update(float deltaTime) override;
		void OnEnter() override;
		void OnExit() override;

		std::unique_ptr<StateMachineBase> Notify(const dae::Event& event) override;

	private:
		bool IsDirectionValid(glm::vec3 direction) const;

		bomberman::EnemyData m_EnemyData;
	};
}


