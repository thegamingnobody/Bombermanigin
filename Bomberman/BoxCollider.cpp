#include "BoxCollider.h"
#include <GameObject.h>
#include <Renderer.h>

bomberman::BoxCollider::BoxCollider(dae::GameObject& gameObject, Box hitBox)
	: dae::Component(gameObject)
	, m_HitBox(hitBox)
{
}

void bomberman::BoxCollider::Render() const
{
	glm::vec3 position = GetOwner()->GetTransform()->GetGlobalPosition();
	glm::vec2 size = { m_HitBox.width, m_HitBox.height };
	//glm::vec3 color = { 1.0f, 0.0f, 0.0f }; // Red color
	auto& renderer = dae::Renderer::GetInstance();

	renderer.RenderRect(position.x + m_HitBox.x, position.y + m_HitBox.y, size.x, size.y);

}
