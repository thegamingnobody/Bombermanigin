#ifndef DAE_TIME
#define DAE_TIME

#include "Singleton.h"
#include <chrono>

namespace dae
{
    class TimeManager : public Singleton<TimeManager>
    {
    public:
        void Init();
        void Update();

        float GetDeltaTime() const { return m_DeltaTime; }
        float GetFixedTimeStep() const { return m_FixedTimeStep; }
        std::chrono::milliseconds GetSleepTime();
    private:
        float m_DeltaTime;
        long long m_MsPerFrame;
        float m_FixedTimeStep;

        std::chrono::steady_clock::time_point m_CurrentTime;
        std::chrono::steady_clock::time_point m_LastTime;

    };
}

#endif