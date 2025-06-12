#pragma once
#include "StateMachineBase.h"

namespace bomberman
{
    class GameOverMenuState : public StateMachineBase
    {
    public:
        GameOverMenuState(dae::GameObject& ownerObject);
		~GameOverMenuState() override = default;

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<StateMachineBase> Update(float deltaTime) override;
        std::unique_ptr<StateMachineBase> Notify(const dae::Event& event) override;

    private:
		void CreateGameOverScreen();
    };
}

