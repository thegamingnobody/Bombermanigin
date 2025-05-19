#pragma once
#include "StateMachineBase.h"
#include "Observer.h"
#include "EnemyManager.h"


namespace bomberman
{
	class RoamingState : public StateMachineBase, public dae::Observer
	{
	public:
		RoamingState(dae::GameObject& ownerObject);
		~RoamingState() override = default;

		RoamingState(const RoamingState&) = delete;
		RoamingState(RoamingState&&) = delete;
		RoamingState& operator=(const RoamingState&) = delete;
		RoamingState& operator=(RoamingState&&) = delete;

		std::unique_ptr<StateMachineBase> Update(float deltaTime) override;
		void OnEnter() override;
		void OnExit() override;

		void Notify(const dae::Event& event) override;

	private:
		void FlipDirection();

		glm::vec3 m_Direction;
		bool m_HasDied{ false };
		bomberman::EnemyData m_EnemyData;
	};
}

