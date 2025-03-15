#ifndef SCORECOMPONENT
#define SCORECOMPONENT

#include "Component.h"
#include "Observer.h"


namespace dae
{
    class ScoreComponent : public Component, public Observer
    {
    public:
        ScoreComponent(GameObject& ownerObject);

        void SetScore(int newScore) { m_Score = newScore; }
        int GetScore() const { return m_Score; }

        void AddScore(int addedScore);
        void RemoveScore(int removedScore);

        void Notify(const Event& event) override;

    private:
        int m_Score;
    };
}

#endif