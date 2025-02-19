#ifndef TRANSFORMCOMPONENT
#define TRANSFORMCOMPONENT

#include <glm.hpp>
#include "Component.h"
#include "GameObject.h"

namespace dae
{
	class Transform : public Component
	{
	public:
		Transform(GameObject* ownerObject, float x, float y, float z);
		Transform(GameObject* ownerObject, glm::vec3 position);
		Transform(GameObject* ownerObject, glm::vec2 position);

		virtual ~Transform() = default;
		Transform(const Transform& other) = delete;
		Transform(Transform&& other) = delete;
		Transform& operator= (const Transform& other) = delete;
		Transform& operator=(Transform&& other) = delete;

		const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
		const glm::vec3& GetGlobalPosition() const { return m_GlobalPosition; }

		void SetLocalPosition(float x, float y, float z);
		void SetLocalPosition(glm::vec3 pos);
		void SetLocalPosition(glm::vec2 pos);

		void SetParentPosition(float x, float y, float z);
		void SetParentPosition(glm::vec3 pos);
		void SetParentPosition(glm::vec2 pos);

		void UpdateGlobalPosition();

		void Update(float const deltaTime) override;

	private:
		glm::vec3 m_LocalPosition;
		glm::vec3 m_ParentPosition;
		glm::vec3 m_GlobalPosition;

		bool m_PositionDirty{ false };
	};
}

#endif