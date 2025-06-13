#pragma once
#include <Singleton.h>
#include <vector>
#include <string>

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

        std::string GetName() const;

    private:
        void UpdateLetterDisplay() const;

        std::vector<char> m_Name;
        int m_CurrentIndex;
        int const m_MaxIndex{ 2 };
    };
}