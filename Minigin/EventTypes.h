#ifndef DAE_EVENTTYPES
#define DAE_EVENTTYPES

#include <tuple>
#include <glm.hpp>
#include "GameObject.h"

namespace dae
{
	//Use EventArgumentMasks<EventType::T>::Create() to create the arguments tuple and get intelisense on the parameter/arguments
	class GameObject;

	//enum class EventType
	//{
	//	BOMB_EXPLODED,
	//	OBJECT_DAMAGED,
	//	SCORE_ADDED,
	//	PLAYER_DIED
	//};

	//template<EventType>
	//struct EventArgumentMasks;

	//template<>
	//struct EventArgumentMasks<EventType::BOMB_EXPLODED>
	//{
	//	using Args = std::tuple<glm::vec3, float, GameObject*>;

	//	static Args Create(glm::vec3 position, float radius, GameObject* ownerOfBomb)
	//	{
	//		return std::make_tuple(position, radius, ownerOfBomb);
	//	}
	//};

	//template<>
	//struct EventArgumentMasks<EventType::OBJECT_DAMAGED>
	//{
	//	using Args = std::tuple<GameObject*, int>;

	//	static Args Create(GameObject* objectDamaged, int newCurrentHealth)
	//	{
	//		return std::make_tuple(objectDamaged, newCurrentHealth);
	//	}
	//};

	//template<>
	//struct EventArgumentMasks<EventType::SCORE_ADDED>
	//{
	//	using Args = std::tuple<GameObject*, int>;

	//	static Args Create(GameObject* objectScoreAdded, int newScoreValue)
	//	{
	//		return std::make_tuple(objectScoreAdded, newScoreValue);
	//	}
	//};
}

#endif