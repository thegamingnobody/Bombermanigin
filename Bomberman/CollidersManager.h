#pragma once
#include <Singleton.h>
#include <vector>
#include "BaseCollider.h"
#include <unordered_map>
#include <functional>

namespace bomberman
{
    using CollisionHandler = std::function<void(BaseCollider* self, BaseCollider* other)>;

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

		void SetCollisionHandler(bomberman::CollisionType sourceType, bomberman::CollisionType otherType, CollisionHandler handler);

		const std::vector<bomberman::BaseCollider*>& GetColliders() const { return m_Colliders; }

		void HandleCollision(bomberman::BaseCollider* self, bomberman::BaseCollider* other);

    private:
		std::vector<bomberman::BaseCollider*> m_Colliders{};

		std::unordered_map<CollisionKey, CollisionHandler, CollisionKeyHash> m_CollisionHandlers{};
    };
}