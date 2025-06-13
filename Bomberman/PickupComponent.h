#pragma once
#include <Component.h>
#include <GameObject.h>
#include "PickupBase.h"

namespace bomberman
{
	class PickupComponent final : public dae::Component
	{
	public:
		PickupComponent(dae::GameObject& owner, std::shared_ptr<PickupBase> pickup);
		~PickupComponent() = default;

		void Update(float const /*deltaTime*/) override {};
		void Render() const override {};
		void RenderImGui() override {};

		void CollectPickup(int playerId);

	private:
		std::shared_ptr<PickupBase> m_Pickup;
	};
}

