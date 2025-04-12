#ifndef SCORECOMPONENT
#define SCORECOMPONENT

#include "Component.h"
#include "Observer.h"


namespace bomberman
{
    class ScoreComponent : public dae::Component, public dae::Observer
    {
    public:
        ScoreComponent(dae::GameObject& ownerObject);

        void SetScore(int newScore) { m_Score = newScore; }
        int GetScore() const { return m_Score; }

        void AddScore(int addedScore);
        void RemoveScore(int removedScore);

        void Notify(const dae::Event& event) override;

    private:
        int m_Score;
    };
}

#endif