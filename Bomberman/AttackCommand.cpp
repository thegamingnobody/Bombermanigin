#include "AttackCommand.h"
#include "BombExplodedEvent.h"
#include "EventManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "BombComponent.h"

bomberman::AttackCommand::AttackCommand(dae::GameObject& controllingObject)
	: m_pControllingObject(&controllingObject)
{
}

void bomberman::AttackCommand::Execute()
{
	glm::vec3 position = m_pControllingObject->GetTransform()->GetGlobalPosition();
	SpawnBombObject(position);
}

void bomberman::AttackCommand::SpawnBombObject(glm::vec3 position)
{
	//Todo: spawn bomb
	auto activeScene = dae::SceneManager::GetInstance().GetScene("Game");

	if (!activeScene) return;

	auto bomb = std::make_shared<dae::GameObject>("Bomb", position);
	auto& textureComponent = bomb->AddComponent<dae::TextureComponent>(*bomb.get());
	textureComponent.AddTexture("Bomb_1.png");
	textureComponent.AddTexture("Bomb_2.png");
	textureComponent.AddTexture("Bomb_3.png");
	textureComponent.SetCurrentIndex(0);
	bomb->AddComponent<bomberman::BombComponent>(*bomb.get(), 1, 3.0f);

	activeScene->Add(bomb);
}
