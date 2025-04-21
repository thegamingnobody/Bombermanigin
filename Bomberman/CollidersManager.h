#pragma once
#include <Singleton.h>
#include <vector>
#include "BaseCollider.h"

namespace bomberman
{
    class CollidersManager : public dae::Singleton<CollidersManager>
    {
    public:

        void AddCollider(bomberman::BaseCollider& collider);
		void RemoveCollider(bomberman::BaseCollider& collider);

		const std::vector<bomberman::BaseCollider*>& GetColliders() const { return m_Colliders; }

    private:
		std::vector<bomberman::BaseCollider*> m_Colliders{};
		//std::vector<std::shared_ptr<dae::GameObject>> m_Triggers{};
    };
}