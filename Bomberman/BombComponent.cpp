#include "BombComponent.h"
#include "GameObject.h"
#include <SceneManager.h>
#include <Scene.h>
#include "ExplosionComponent.h"
#include <TextureComponent.h>
#include "Grid.h"
#include "BoxCollider.h"
#include "BombExplodedEvent.h"
#include <EventManager.h>
#include "SceneNames.h"

bomberman::BombComponent::~BombComponent()
{
	auto owner = GetOwner();

	BombExplodedEvent event = BombExplodedEvent(glm::vec3(0.0f, 0.0f, 0.0f), 1, owner);
	dae::EventManager::GetInstance().BroadcastEvent(std::move(std::make_unique<BombExplodedEvent>(event)));
}

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
	auto activeScene = dae::SceneManager::GetInstance().GetScene(SCENE_OBJECTS);
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

	explosion->AddComponent<bomberman::BoxCollider>(*explosion.get(), bomberman::CollisionType::Explosion, horizontalLine);
	explosion->AddComponent<bomberman::BoxCollider>(*explosion.get(), bomberman::CollisionType::Explosion, verticalLine);
	auto& textureComponent = explosion->AddComponent<dae::TextureComponent>(*explosion.get());
	textureComponent.AddTexture("Explosion_1_1.png");
	activeScene->Add(explosion);
}
