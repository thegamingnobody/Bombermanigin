#include "BrickDestroyedState.h"
#include "Grid.h"

bomberman::BrickDestroyedState::BrickDestroyedState(dae::GameObject& ownerObject)
	: StateMachineBase(ownerObject)
{
}

void bomberman::BrickDestroyedState::OnEnter()
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::BrickDestroyedState::Update(float /*deltaTime*/)
{
	//Todo: animate destruction
	//When animation is compolete, remove the brick
	auto& grid = bomberman::Grid::GetInstance();
	auto position = grid.WorldPosToGridCoord(m_Owner->GetTransform()->GetGlobalPosition());

	grid.BrickDestroyed(grid.GetCellID(static_cast<int>(position.x), static_cast<int>(position.y)));

	m_Owner->SetShouldBeRemoved();

	return nullptr;
}

void bomberman::BrickDestroyedState::OnExit()
{
}

std::unique_ptr<bomberman::StateMachineBase> bomberman::BrickDestroyedState::Notify(const dae::Event& /*event*/)
{
	return nullptr;
}
