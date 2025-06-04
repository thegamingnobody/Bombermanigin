#pragma once
#include "StateMachineBase.h"

namespace bomberman
{
    class MainGameState : public StateMachineBase
    {
    public:
        MainGameState(dae::GameObject& ownerObject);
		~MainGameState() override = default;

		void OnEnter() override;
		void OnExit() override;

		std::unique_ptr<StateMachineBase> Update(float deltaTime) override;
        std::unique_ptr<StateMachineBase> Notify(const dae::Event& event) override;
    
    };
}


