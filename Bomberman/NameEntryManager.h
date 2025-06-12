#pragma once
#include <Singleton.h>
#include <vector>


namespace bomberman
{
    class NameEntryManager : public dae::Singleton<NameEntryManager>
    {
    public:
        void Init();
		void Reset();

        void MoveRight();
        void MoveLeft();

        void IncrementLetter();
        void DecrementLetter();

		void ConfirmName();

    private:
        void UpdateLetterDisplay() const;

        std::vector<char> m_Name;
        int m_CurrentIndex;
        int const m_MaxIndex{2};
    };
}

