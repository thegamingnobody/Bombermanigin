#pragma once
#include "StateMachineBase.h"

namespace bomberman
{
    class GameOverMenuState : public StateMachineBase
    {
    public:
        using ScoreEntry = std::pair<std::string, int>;

        GameOverMenuState(dae::GameObject& ownerObject);
		~GameOverMenuState() override = default;

        void OnEnter() override;
        void OnExit() override;

        std::unique_ptr<StateMachineBase> Update(float deltaTime) override;
        std::unique_ptr<StateMachineBase> Notify(const dae::Event& event) override;

    private:
		void CreateGameOverScreen();

		void SortAndTrimScores();

		void LoadScores();
		void SaveScores(ScoreEntry newScoreEntry = ScoreEntry());

        std::vector<ScoreEntry> m_Scores;
        const size_t m_MaxScores = 5;
    };
}

