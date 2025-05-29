#pragma once
#include "StateMachineBase.h"
#include "EnemyManager.h"


namespace bomberman
{
	class RoamingState : public StateMachineBase
	{
	public:
		RoamingState(dae::GameObject& ownerObject, const bomberman::EnemyData& enemyData);
		~RoamingState() override = default;

		RoamingState(const RoamingState&) = delete;
		RoamingState(RoamingState&&) = delete;
		RoamingState& operator=(const RoamingState&) = delete;
		RoamingState& operator=(RoamingState&&) = delete;

		std::unique_ptr<StateMachineBase> Update(float deltaTime) override;
		void OnEnter() override;
		void OnExit() override;

		std::unique_ptr<StateMachineBase> Notify(const dae::Event& event) override;

	private:
		void FlipDirection();

		glm::vec3 m_Direction;
		bomberman::EnemyData m_EnemyData;
	};
}

