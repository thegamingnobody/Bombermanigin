#include "Component.h"

dae::Component::Component(dae::GameObject* ownerObject, ComponentType compnentType)
	: m_pOwnerObject(ownerObject)
	, m_ComponentType(compnentType)
{
}