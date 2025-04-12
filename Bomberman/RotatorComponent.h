#ifndef ROTATORCOMPONENT
#define ROTATORCOMPONENT

#include "PhysicsComponent.h"
#include <glm.hpp>
#include "GameObject.h"

namespace bomberman
{
    class RotatorComponent : public dae::PhysicsComponent
    {
    public:
        RotatorComponent(dae::GameObject& ownerObject, float speed = 1.0f, float radius = 10.0f);

        virtual ~RotatorComponent() = default;
        RotatorComponent(const RotatorComponent& other) = delete;
        RotatorComponent(RotatorComponent&& other) = delete;
        RotatorComponent& operator= (const RotatorComponent& other) = delete;
        RotatorComponent& operator=(RotatorComponent&& other) = delete;

        virtual void Update(float const deltaTime) override;

    private:
		float m_Speed;
        float m_CurrentAngle;
        float m_Radius;
    };
}

#endif