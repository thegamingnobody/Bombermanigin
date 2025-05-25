#include "AttackCommand.h"
#include "BombExplodedEvent.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "BombComponent.h"
#include "PlayerManager.h"

bomberman::AttackCommand::AttackCommand(dae::GameObject& controllingObject)
	: m_pControllingObject(&controllingObject)
{
	dae::EventManager::GetInstance().AddObserver(*this, static_cast<int>(bomberman::EventType::BOMB_EXPLODED));
}

void bomberman::AttackCommand::Execute()
{
	auto& playerManager = bomberman::PlayerManager::GetInstance();

	if (m_BombCount >= playerManager.GetPlayerInfo(0).maxBombs) return;

	glm::vec3 position = m_pControllingObject->GetTransform()->GetGlobalPosition();
	SpawnBombObject(position);
}

void bomberman::AttackCommand::SpawnBombObject(glm::vec3 position)
{
	auto activeScene = dae::SceneManager::GetInstance().GetScene("Objects");

	if (!activeScene) return;

	auto bomb = std::make_shared<dae::GameObject>("Bomb", position);
	auto& textureComponent = bomb->AddComponent<dae::TextureComponent>(*bomb.get());
	textureComponent.AddTexture("Bomb_1.png");
	textureComponent.AddTexture("Bomb_2.png");
	textureComponent.AddTexture("Bomb_3.png");
	textureComponent.SetCurrentIndex(0);
	bomb->AddComponent<bomberman::BombComponent>(*bomb.get(), 1, 3.0f, 0.5f);

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