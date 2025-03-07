#ifndef TRANSFORMCOMPONENT
#define TRANSFORMCOMPONENT

#include <glm.hpp>
#include "Component.h"

namespace dae
{
	class Transform : public Component
	{
	public:
		Transform(GameObject& ownerObject, float x, float y, float z);
		Transform(GameObject& ownerObject, glm::vec3 position);
		Transform(GameObject& ownerObject, glm::vec2 position);

		~Transform() = default;
		Transform(const Transform& other) = delete;
		Transform(Transform&& other) = delete;
		Transform& operator= (const Transform& other) = delete;
		Transform& operator=(Transform&& other) = delete;

		const glm::vec3& GetLocalPosition() const;
		const glm::vec3& GetGlobalPosition();

		void SetLocalPosition(float x, float y, float z = 0.0f);
		void SetLocalPosition(glm::vec3 pos);
		void SetLocalPosition(glm::vec2 pos);

		void SetParentPosition(float x, float y, float z = 0.0f);
		void SetParentPosition(glm::vec3 pos);
		void SetParentPosition(glm::vec2 pos);

		void SetDirty();

		void UpdateGlobalPosition();

		void Move(float x, float y, float z = 0.0f);
		void Move(glm::vec3 pos);
		void Move(glm::vec2 pos);

	private:
		glm::vec3 m_LocalPosition;
		glm::vec3 m_ParentPosition;
		glm::vec3 m_GlobalPosition;

		bool m_ShouldUpdatePosition{ false };
	};
}

#endif