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
#include <ServiceLocator.h>
#include "SoundIds.h"
#include "PlayerManager.h"
#include <sstream>

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
	position.x -= size * TILE_SIZE;
	position.y -= size * TILE_SIZE;

	auto explosion = std::make_shared<dae::GameObject>("Explosion", position);
	explosion->AddComponent<bomberman::ExplosionComponent>(*explosion.get(), size, m_ExplosionTime);

	int crossSize = size * 2 + 1;

	float lineWidth = crossSize * TILE_SIZE * 0.8f;
	float lineHeight = TILE_SIZE / 2.0f;

	float centerOffset = size * TILE_SIZE;

	// Horizontal line: spans left-right, centered vertically
	float horizontalX = 4.0f;
	float horizontalY = centerOffset + TILE_SIZE / 2.0f - lineHeight / 2.0f;

	// Vertical line: spans top-bottom, centered horizontally
	float verticalX = centerOffset + TILE_SIZE / 2.0f - lineHeight / 2.0f;
	float verticalY = 0.0f;

	Box horizontalLine{ horizontalX, horizontalY, lineWidth, lineHeight };
	Box verticalLine{ verticalX, verticalY, lineHeight, lineWidth };
	explosion->AddComponent<bomberman::BoxCollider>(*explosion.get(), bomberman::CollisionType::Explosion, horizontalLine);
	explosion->AddComponent<bomberman::BoxCollider>(*explosion.get(), bomberman::CollisionType::Explosion, verticalLine);
	auto& textureComponent = explosion->AddComponent<dae::TextureComponent>(*explosion.get());

	std::stringstream ss;

	ss << "Explosion_" << size << "_" << 1 << ".png";

	textureComponent.AddTexture(ss.str());
	activeScene->Add(explosion);

	float volume = 0.15f;
	dae::ServiceLocator::GetSoundSystem().PlaySound(static_cast<int>(bomberman::SoundId::Explosion), volume, -1);

}
