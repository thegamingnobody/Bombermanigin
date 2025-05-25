#ifndef HEALTHCOMPONENT
#define HEALTHCOMPONENT

#include "Component.h"
#include "Observer.h"
#include <Event.h>

namespace bomberman
{
	class HealthComponent : public dae::Component
	{
	public:
		HealthComponent(dae::GameObject& ownerObject, int maxHealth, bool canSurpassMax = false);

		int GetMaxHealth() const { return m_MaxHealth; }
		int GetCurrentHealth() const { return m_CurrentHealth; }

		void Heal(int amount);
		void Damage(int amount);

	private:
		int m_MaxHealth;
		int m_CurrentHealth;
		bool m_CanSurpassMax;
	};
}

#endif