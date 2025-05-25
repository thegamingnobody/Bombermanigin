#include "BombComponent.h"
#include "GameObject.h"
#include <SceneManager.h>
#include <Scene.h>
#include "ExplosionComponent.h"
#include <TextureComponent.h>
#include "Grid.h"
#include "BoxCollider.h"

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

	int crossSize = 3;

	float lineWidth = crossSize * TILE_SIZE * 0.8f;
	float lineHeight = TILE_SIZE / 2;
	float lineX = crossSize * 0.1f * TILE_SIZE;
	float lineY = (TILE_SIZE * crossSize * 0.41f);

	Box horizontalLine{ lineX, lineY, lineWidth, lineHeight };
	Box verticalLine{ lineY, lineX, lineHeight,	lineWidth };

	explosion->AddComponent<bomberman::BoxCollider>(*explosion.get(), bomberman::CollisionType::Bomb, horizontalLine);
	explosion->AddComponent<bomberman::BoxCollider>(*explosion.get(), bomberman::CollisionType::Bomb, verticalLine);
	auto& textureComponent = explosion->AddComponent<dae::TextureComponent>(*explosion.get());
	textureComponent.AddTexture("Explosion_1_1.png");
	activeScene->Add(explosion);
}
