#include "AttackCommand.h"
#include "BombExplodedEvent.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "BombComponent.h"
#include "PlayerManager.h"
#include "Grid.h"
#include "BoxCollider.h"
#include "SceneNames.h"

//Todo: limit to player bomb limit

bomberman::AttackCommand::AttackCommand(dae::GameObject& controllingObject, int playerNumber)
	: m_pControllingObject(&controllingObject)
	, m_PlayerNumber(playerNumber)
{
	dae::EventManager::GetInstance().AddObserver(*this, static_cast<int>(bomberman::EventType::BOMB_EXPLODED));
}

void bomberman::AttackCommand::Execute()
{
	auto& playerManager = bomberman::PlayerManager::GetInstance();

	if (m_BombCount >= playerManager.GetPlayerInfo(m_PlayerNumber).maxBombs) return;

	glm::vec3 position = m_pControllingObject->GetTransform()->GetGlobalPosition();
	SpawnBombObject(position);
}

void bomberman::AttackCommand::SpawnBombObject(glm::vec3 position)
{
	auto activeScene = dae::SceneManager::GetInstance().GetScene(SCENE_OBJECTS);

	if (!activeScene) return;

	auto& grid = bomberman::Grid::GetInstance();

	auto gridCoord = grid.WorldPosToGridCoord(position.x + (TILE_SIZE / 2), position.y + (TILE_SIZE / 2));
	auto gridPos = grid.GridCoordToWorldPos(static_cast<int>(gridCoord.x), static_cast<int>(gridCoord.y));

	auto bomb = std::make_shared<dae::GameObject>("Bomb", gridPos);
	auto& textureComponent = bomb->AddComponent<dae::TextureComponent>(*bomb.get());
	textureComponent.AddTexture("Bomb_1.png");
	textureComponent.AddTexture("Bomb_2.png");
	textureComponent.AddTexture("Bomb_3.png");
	textureComponent.SetCurrentIndex(0);
	bomb->AddComponent<bomberman::BombComponent>(*bomb.get(), 1, 3.0f, 0.5f);
	bomb->AddComponent<bomberman::BoxCollider>(*bomb.get(), CollisionType::Bomb);

	activeScene->Add(bomb);
	m_BombCount++;
}

void bomberman::AttackCommand::Notify(const dae::Event& event)
{
	switch (static_cast<bomberman::EventType>(event.GetEventType()))
	{
	case bomberman::EventType::BOMB_EXPLODED:
	{
		m_BombCount--;
		break;
	}
	default:
		break;
	}

}