#ifndef BOMBEXPLODEDEVENT
#define BOMBEXPLODEDEVENT

#include <Event.h>
#include <glm.hpp>
#include <GameObject.h>
#include "EventTypes.h"

namespace bomberman
{
	class BombExplodedEvent : public dae::Event
	{
	public:
		BombExplodedEvent(const glm::vec3& position, float radius, dae::GameObject* ownerOfBomb)
			: Event(static_cast<int>(bomberman::EventType::BOMB_EXPLODED))
			, m_Position(position)
			, m_Radius(radius)
			, m_OwnerOfBomb(ownerOfBomb)
		{
		}

		glm::vec3 GetPosition() const { return m_Position; }

		float GetRadius() const { return m_Radius; }

		dae::GameObject* GetOwnerOfBomb() const { return m_OwnerOfBomb; }

	private:
		glm::vec3 m_Position;
		float m_Radius;
		dae::GameObject* m_OwnerOfBomb;
	};
}

#endif