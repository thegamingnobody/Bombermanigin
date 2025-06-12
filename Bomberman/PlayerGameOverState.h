#pragma once
#include "StateMachineBase.h"

namespace bomberman
{
    class PlayerGameOverState : public StateMachineBase
    {
    public:
        PlayerGameOverState(dae::GameObject& ownerObject, int playerID);
        ~PlayerGameOverState() override = default;
    
        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<StateMachineBase> Update(float deltaTime) override;
        std::unique_ptr<StateMachineBase> Notify(const dae::Event& event) override;

    private:
        int m_PlayerID;
    };
}


