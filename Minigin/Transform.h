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
		glm::vec3 GetPredictedPosition();

		void SetLocalPosition(float x, float y, float z = 0.0f);
		void SetLocalPosition(glm::vec3 pos);
		void SetLocalPosition(glm::vec2 pos);

		void SetDirty();

		void UpdateGlobalPosition();

		void Move(float x, float y, float z = 0.0f);
		void Move(glm::vec3 pos);
		void Move(glm::vec2 pos);

		void LateUpdate() override;
		void ResetMovementThisFrame();

	private:
		glm::vec3 CalculateGlobalPosition() const;

		glm::vec3 m_LocalPosition;
		glm::vec3 m_GlobalPosition;

		glm::vec3 m_MovementThisFrame;

		bool m_ShouldUpdatePosition{ false };
	};
}

#endif