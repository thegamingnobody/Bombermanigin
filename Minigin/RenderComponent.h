#ifndef RENDERCOMPONENT
#define RENDERCOMPONENT

#include "Component.h"

namespace dae
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent(dae::GameObject* ownerObject, ComponentType compnentType = ComponentType::RenderComponent);

		virtual ~RenderComponent() = default;
		RenderComponent(const RenderComponent& other) = delete;
		RenderComponent(RenderComponent&& other) = delete;
		RenderComponent& operator= (const RenderComponent& other) = delete;
		RenderComponent& operator=(RenderComponent&& other) = delete;

		virtual void Update(float const deltaTime) = 0;
		virtual void Render() const = 0;

	private:
	};
}

#endif