#pragma once
#include "StateMachineBase.h"
#include <Scene.h>

namespace bomberman
{
    class LoadLevelState : public StateMachineBase
    {
    public:
        LoadLevelState(dae::GameObject& ownerObject);
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
    };
}


