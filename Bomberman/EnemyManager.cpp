#include "EnemyManager.h"  
#include <json.hpp>
#include <fstream>
#include <GameObject.h>
#include "BoxCollider.h"
#include <TextComponent.h>
#include "StateMachineComponent.h"
#include "RoamingState.h"
#include "HealthComponent.h"

using json = nlohmann::json;  

void bomberman::EnemyManager::Init()  
{
	std::filesystem::path filePath = __FILE__;
	std::filesystem::path fileDir = filePath.parent_path();
	auto dataPath = "EnemyData\\EnemyData.json";

	fileDir.append(dataPath);
	std::ifstream f(fileDir.c_str());
	std::ifstream f2(dataPath);
	json data;
	try
	{
		data = json::parse(f);
	}
	catch (...)
	{
		data = json::parse(f2);
	}

	for (auto& enemy : data)
	{
		EnemyData enemyData;
		enemyData.name = enemy["Name"];
		enemyData.speed = enemy["Speed"];
		enemyData.score = enemy["Score"];
		enemyData.health = enemy["Health"];
		enemyData.minLevel = enemy["MinLevel"];
		enemyData.intelligence = enemy["Intelligence"];
		enemyData.chasePlayer = enemy["ChasePlayer"];
		enemyData.detectionRange = enemy["DetectRange"];

		m_EnemyData.emplace_back(enemyData);
	}
}

std::shared_ptr<dae::GameObject> bomberman::EnemyManager::CreateEnemy(bomberman::EnemyType enemyType, glm::vec3 position)
{
	if (static_cast<int>(enemyType) > static_cast<int>(m_EnemyData.size())) return nullptr;

	auto go = std::make_shared<dae::GameObject>(m_EnemyData[static_cast<int>(enemyType)].name, position);

	auto& stateMachineComponent = go->AddComponent<bomberman::StateMachineComponent>(*go.get());

	auto roamingState = std::make_unique<bomberman::RoamingState>(*go.get());
	stateMachineComponent.ChangeState(std::move(roamingState));

	std::stringstream ss;

	//Todo: replace "E" and "1"  if animation is added
	ss << m_EnemyData[static_cast<int>(enemyType)].name << "_" << "E" << "_" << "1" << ".png";

	go->AddComponent<dae::TextureComponent>(*go.get()).AddTexture(ss.str());

	go->AddComponent<bomberman::HealthComponent>(*go.get(), m_EnemyData[static_cast<int>(enemyType)].health);

	float offset{ 2.0f };
	go->AddComponent<bomberman::BoxCollider>(*go.get(), bomberman::CollisionType::Enemy, bomberman::Box(offset, offset, TILE_SIZE - (2 * offset), TILE_SIZE - (2 * offset)));

	m_EnemyCount++;

	return go;
}

bomberman::EnemyData bomberman::EnemyManager::GetEnemyData(EnemyType enemyType) const
{
	EnemyData result{};

	if (static_cast<int>(enemyType) > static_cast<int>(m_EnemyData.size())) return result;

	result = m_EnemyData[static_cast<int>(enemyType)];

	return result;
}

int bomberman::EnemyManager::GetEnemySpeed(EnemyType enemyType) const
{
	return GetEnemyData(enemyType).speed;
}
int bomberman::EnemyManager::GetEnemyScore(EnemyType enemyType) const
{
	return GetEnemyData(enemyType).score;
}
int bomberman::EnemyManager::GetEnemyHealth(EnemyType enemyType) const
{
	return GetEnemyData(enemyType).health;
}
int bomberman::EnemyManager::GetEnemyMinLevel(EnemyType enemyType) const
{
	return GetEnemyData(enemyType).minLevel;
}
int bomberman::EnemyManager::GetEnemyIntelligence(EnemyType enemyType) const
{
	return GetEnemyData(enemyType).intelligence;
}
bool bomberman::EnemyManager::GetEnemyChasePlayer(EnemyType enemyType) const
{
	return GetEnemyData(enemyType).chasePlayer;
}

void bomberman::EnemyManager::EnemyDied()
{
	m_EnemyCount--;
}
bool bomberman::EnemyManager::AreAllEnemiesDead() const
{
	return (m_EnemyCount == 0);
}
