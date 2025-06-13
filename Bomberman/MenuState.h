#pragma once
#include "StateMachineBase.h"

namespace bomberman
{
    class MenuState : public StateMachineBase
    {
    public:
        using ScoreEntry = std::pair<std::string, int>;

        MenuState(dae::GameObject& ownerObject);
        ~MenuState() override = default;
    
        void OnEnter() override;
        void OnExit() override;
        std::unique_ptr<StateMachineBase> Update(float deltaTime) override;

        std::unique_ptr<StateMachineBase> Notify(const dae::Event& event) override;

    private:
        void CreateMenu();
        
        ScoreEntry GetHighScore();
    };
}

