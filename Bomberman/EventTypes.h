#ifndef BOMBERMAN_EVENTTYPES
#define BOMBERMAN_EVENTTYPES

namespace bomberman
{
	enum class EventType
	{
		BOMB_EXPLODED = 0,
		OBJECT_DAMAGED = 1,
		SCORE_ADDED = 2,
		PLAYER_DIED = 3,
		ENEMY_COLLISION = 4,
	};
}

#endif