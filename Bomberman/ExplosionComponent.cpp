#include "ExplosionComponent.h"
#include <SceneManager.h>
#include <Scene.h>
#include <GameObject.h>

void bomberman::ExplosionComponent::Update(float deltaTime)
{
	if (m_ExplosionTime > 0.0f)
	{
		m_ExplosionTime -= deltaTime;
	}
	else
	{
		GetOwner()->SetShouldBeRemoved();
	}
}
