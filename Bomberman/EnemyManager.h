#pragma once
#include <Singleton.h>
#include <vector>
#include <string>
#include <memory>
#include <GameObject.h>

namespace bomberman
{
	enum class EnemyType
	{
		Balloom,
		Oneal,
		Doll,
		Minvo
	};

	struct EnemyData
	{
		std::string name{};
		int speed{};
		int score{};
		int health{};
		int minLevel{};
		int intelligence{};
		bool chasePlayer{};
	};

	class EnemyManager : public dae::Singleton<EnemyManager>
    {
    public:
		void Init();

		std::shared_ptr<dae::GameObject> CreateEnemy(bomberman::EnemyType enemyType, glm::vec3 position);

		EnemyData GetEnemyData(EnemyType enemyType) const;
		int GetEnemySpeed(EnemyType enemyType) const;
		int GetEnemyScore(EnemyType enemyType) const;
		int GetEnemyHealth(EnemyType enemyType) const;
		int GetEnemyMinLevel(EnemyType enemyType) const;
		int GetEnemyIntelligence(EnemyType enemyType) const;
		bool GetEnemyChasePlayer(EnemyType enemyType) const;

    private:
		std::vector<EnemyData> m_EnemyData;
    };
}

