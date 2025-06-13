#pragma once
#include <Event.h>

namespace bomberman
{
    class NameConfirmEvent : public dae::Event
    {
    public:
        NameConfirmEvent(std::string name);
		~NameConfirmEvent() = default;

		std::string GetName() const { return m_Name; }

    private:
        std::string m_Name;
    };
}


