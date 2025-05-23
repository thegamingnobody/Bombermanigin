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
	auto activeScene = dae::SceneManager::GetInstance().GetScene("Objects");
	if (!activeScene) return;

	auto position = GetOwner()->GetTransform()->GetGlobalPosition();
	position.x -= TILE_SIZE;
	position.y -= TILE_SIZE;

	auto explosion = std::make_shared<dae::GameObject>("Explosion", position);
	explosion->AddComponent<bomberman::ExplosionComponent>(*explosion.get(), size, m_ExplosionTime);
	explosion->AddComponent<bomberman::CrossCollider>(*explosion.get(), bomberman::CollisionType::Bomb);
	auto& textureComponent = explosion->AddComponent<dae::TextureComponent>(*explosion.get());
	textureComponent.AddTexture("Explosion_1_1.png");
	//textureComponent.SetOffset(glm::vec2(-1 * TILE_SIZE, -1 * TILE_SIZE));
	activeScene->Add(explosion);
}
