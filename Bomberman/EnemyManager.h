#pragma once
#include <Singleton.h>
#include <vector>
#include <string>

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
		std::string name;
		int speed;
		int score;
		int health;
		int minLevel;
		int intelligence;
	};

	class EnemyManager : public dae::Singleton<EnemyManager>
    {
    public:
		void Init();

		const EnemyData& GetEnemyData(EnemyType enemyType) const;
		int GetEnemySpeed(EnemyType enemyType) const;
		int GetEnemyScore(EnemyType enemyType) const;
		int GetEnemyHealth(EnemyType enemyType) const;
		int GetEnemyMinLevel(EnemyType enemyType) const;
		int GetEnemyIntelligence(EnemyType enemyType) const;

    private:
		std::vector<EnemyData> m_EnemyData;
    };
}

