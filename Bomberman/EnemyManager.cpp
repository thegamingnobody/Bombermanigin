#include "EnemyManager.h"  
#include <json.hpp>
#include <fstream>

using json = nlohmann::json;  

void bomberman::EnemyManager::Init()  
{
	std::filesystem::path filePath = __FILE__;
	std::filesystem::path fileDir = filePath.parent_path();
	auto dataPath = "EnemyData\\enemyData.json";

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

		m_EnemyData.emplace_back(enemyData);
	}
}

const bomberman::EnemyData& bomberman::EnemyManager::GetEnemyData(EnemyType enemyType) const
{
	EnemyData result;

	if (static_cast<int>(enemyType) > m_EnemyData.size()) return result;

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
