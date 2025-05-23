#pragma once
#include <Singleton.h>
#include <vector>
#include "BaseCollider.h"
#include <unordered_map>
#include <functional>

namespace bomberman
{
	// The function/lambda that will be called when a collision occurs
    using CollisionHandler = std::function<void(BaseCollider* self, BaseCollider* other)>;

	// The keys used to call the correct collision handler
    struct CollisionKey 
    {
        bomberman::CollisionType sourceType;
        bomberman::CollisionType otherType;

        CollisionKey(bomberman::CollisionType source, bomberman::CollisionType other) 
			: sourceType(source)
            , otherType(other) 
        {
		}

        bool operator==(const CollisionKey& other) const 
        {
            return ((sourceType == other.sourceType) and (otherType == other.otherType));
        }

        CollisionKey Reverse() const 
        {
            return CollisionKey(otherType, sourceType);
		}
    };

	// The hash needed to use the CollisionKey in an unordered_map
    struct CollisionKeyHash 
    {
        std::size_t operator()(const CollisionKey& key) const 
        {
            std::size_t hash1 = std::hash<int>{}(static_cast<int>(key.sourceType));
            std::size_t hash2 = std::hash<int>{}(static_cast<int>(key.otherType) << 1);

            return (hash1 ^ hash2);
        }
    };

    class CollidersManager : public dae::Singleton<CollidersManager>
    {
    public:
        void Init();

        void AddCollider(bomberman::BaseCollider& collider);
		void RemoveCollider(bomberman::BaseCollider& collider);

		void SetCollisionHandler(bomberman::CollisionType sourceType, bomberman::CollisionType otherType, CollisionHandler handler, bool alsoSetReverse);

		const std::vector<bomberman::BaseCollider*>& GetColliders() const { return m_Colliders; }

		void HandleCollision(bomberman::BaseCollider* self, bomberman::BaseCollider* other);

		// Returns true if the given type is in the colliders list as a source type
        bool IsSourceType(bomberman::CollisionType sourceType) const;

        bool IsKeySet(CollisionKey key) const;

    private:
		std::vector<bomberman::BaseCollider*> m_Colliders{};

		std::unordered_map<CollisionKey, CollisionHandler, CollisionKeyHash> m_CollisionHandlers{};
    };
}