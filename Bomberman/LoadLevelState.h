#pragma once
#include "StateMachineBase.h"
#include <Scene.h>
#include "GameManager.h"

namespace bomberman
{
    class LoadLevelState final : public StateMachineBase
    {
    public:
        LoadLevelState(dae::GameObject& ownerObject, GameMode chosenGameMode);
        ~LoadLevelState() override = default;

        void OnEnter() override;
        void OnExit() override;
        std::unique_ptr<StateMachineBase> Update(float deltaTime) override;

        std::unique_ptr<StateMachineBase> Notify(const dae::Event& event) override;

    private:
		void CreateScenes();

        void LoadMap(dae::Scene& scene);
        void LoadPlayer();
        void LoadHud(dae::Scene& scene);

		bool m_ScenesCreated{ false };
        GameMode m_ChosenGameMode;
    };
}


