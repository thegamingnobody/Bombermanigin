#ifndef HEALTHCOMPONENT
#define HEALTHCOMPONENT

#include "Component.h"
#include "Observer.h"

namespace dae
{
	class HealthComponent : public Component, public Observer
	{
	public:
		HealthComponent(GameObject& ownerObject, int maxHealth, bool canSurpassMax = false);

		int GetMaxHealth() const { return m_MaxHealth; }
		int GetCurrentHealth() const { return m_CurrentHealth; }

		void Heal(int amount);
		void Damage(int amount);

		void Notify(const Event& event) override;

	private:
		int m_MaxHealth;
		int m_CurrentHealth;
		bool m_CanSurpassMax;
	};
}

#endif