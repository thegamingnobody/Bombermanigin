#include "BombComponent.h"
#include "GameObject.h"
#include "CrossCollider.h"
#include <SceneManager.h>
#include <Scene.h>
#include "ExplosionComponent.h"
#include <TextureComponent.h>

void bomberman::BombComponent::Update(float deltaTime)
{
	if (!m_Exploded)
	{
		m_TimeToExplode -= deltaTime;
	}

	//Todo: animate bomb (based on time left?)
	if (m_TimeToExplode <= 0.0f)
	{
		if (m_Exploded) return;

		m_Exploded = true;

		SpawnExplosion(m_ExplosionSize);
		GetOwner()->SetShouldBeRemoved();
	}
}

void bomberman::BombComponent::SpawnExplosion(int size)
{
	auto activeScene = dae::SceneManager::GetInstance().GetScene("Game");
	if (!activeScene) return;
	auto explosion = std::make_shared<dae::GameObject>("Explosion", GetOwner()->GetTransform()->GetGlobalPosition());
	explosion->AddComponent<bomberman::ExplosionComponent>(*explosion.get(), size, m_ExplosionTime);
	explosion->AddComponent<bomberman::CrossCollider>(*explosion.get(), bomberman::CollisionType::Bomb);
	auto& textureComponent = explosion->AddComponent<dae::TextureComponent>(*explosion.get());
	textureComponent.AddTexture("Explosion_1_1.png");
	activeScene->Add(explosion);
}
