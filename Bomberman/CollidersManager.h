#pragma once
#include <Singleton.h>
#include <vector>
#include "BoxCollider.h"

namespace bomberman
{
    class CollidersManager : public dae::Singleton<CollidersManager>
    {
    public:

        void AddCollider(bomberman::BoxCollider& collider);
		void RemoveCollider(bomberman::BoxCollider& collider);

		const std::vector<bomberman::BoxCollider*>& GetColliders() const { return m_Colliders; }

    private:
		std::vector<bomberman::BoxCollider*> m_Colliders{};
		//std::vector<std::shared_ptr<dae::GameObject>> m_Triggers{};
    };
}