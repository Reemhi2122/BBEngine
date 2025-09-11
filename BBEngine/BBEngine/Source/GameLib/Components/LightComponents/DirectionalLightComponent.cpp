#include "GameLib/Components/LightComponents/DirectionalLightComponent.h"

DirectionalLightComponent::DirectionalLightComponent(DirectionalLight* a_DirectionalLight, TransformComponent* a_Transform)
{
	m_DirectionalLight = a_DirectionalLight;
	m_Transform = a_Transform;
}

void DirectionalLightComponent::Update(IGraphics& a_Graphics)
{
	//m_DirectionalLight->direction = m_Transform->GetPosition();
}

void DirectionalLightComponent::Draw(IGraphics& a_Graphics) {}

void DirectionalLightComponent::InspectorDrawUI()
{

}