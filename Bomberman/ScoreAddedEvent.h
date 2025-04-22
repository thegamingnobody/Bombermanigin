#ifndef SCOREADDEDEVENT
#define SCOREADDEDEVENT

#include <Event.h>
#include <GameObject.h>
#include "EventTypes.h"

namespace bomberman
{
	class ScoreAddedEvent : public dae::Event
	{
	public:
		ScoreAddedEvent(dae::GameObject* object, int score)
			: Event(static_cast<int>(EventType::SCORE_ADDED))
			, m_Object(object)
			, m_Score(score)
		{
		}

		dae::GameObject* GetObject() const { return m_Object; }

		int GetScore() const { return m_Score; }

	private:
		dae::GameObject* m_Object;
		int m_Score;
	};
}


#endif