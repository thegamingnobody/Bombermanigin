#include "StateMachineComponent.h"

//bomberman::StateMachineComponent::StateMachineComponent(dae::GameObject& ownerObject, std::unique_ptr<StateMachineBase> startingState)
//	: dae::Component(ownerObject)
//	, m_CurrentState(nullptr)
//	, m_EnemyData()
//{
//	ChangeState(std::move(startingState));
//	LoadEnemyData();
//}

bomberman::StateMachineComponent::StateMachineComponent(dae::GameObject& ownerObject)
	: dae::Component(ownerObject)
	, m_CurrentState(nullptr)
	, m_EnemyData()
{
	LoadEnemyData();
}

bomberman::StateMachineComponent::~StateMachineComponent()
{
	if (m_CurrentState)
	{
		m_CurrentState->OnExit();
	}
}

void bomberman::StateMachineComponent::Update(float deltaTime)
{
	if (m_CurrentState)
	{
		auto returnValue = m_CurrentState->Update(deltaTime);

		if (returnValue == nullptr) return;

		ChangeState(std::move(returnValue));
	}
}

void bomberman::StateMachineComponent::ChangeState(std::unique_ptr<StateMachineBase> newState)
{
	if (m_CurrentState)
	{
		m_CurrentState->OnExit();
	}

	m_CurrentState = std::move(newState);

	m_CurrentState->OnEnter();
}

void bomberman::StateMachineComponent::LoadEnemyData()
{
	auto& enemyManager = bomberman::EnemyManager::GetInstance();

	std::string name = GetOwner()->GetName();
	bomberman::EnemyType enemyType{};

	//The reason to use the name instead of a type is because this statemachine component is univeral,
	//so I don't have any enemy specific data or functions here.
	// .find is used because a gameobject gets a random number added to its name when created to avoid (or at least minimize) name collision.
	if (name.find("Balloom") != std::string::npos)
	{
		enemyType = bomberman::EnemyType::Balloom;
	}
	else if (name.find("Oneal") != std::string::npos)
	{
		enemyType = bomberman::EnemyType::Oneal;
	}
	else if (name.find("Doll") != std::string::npos)
	{
		enemyType = bomberman::EnemyType::Doll;
	}
	else if (name.find("Minvo") != std::string::npos)
	{
		enemyType = bomberman::EnemyType::Minvo;
	}

	m_EnemyData = enemyManager.GetEnemyData(enemyType);

}
