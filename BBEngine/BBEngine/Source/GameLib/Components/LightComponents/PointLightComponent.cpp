#include "GameLib/Components/LightComponents/PointLightComponent.h"

PointLightComponent::PointLightComponent(PointLight a_Pointlight, TransformComponent* a_Transform)
{
	m_PointLight = a_Pointlight;
	m_Transform = a_Transform;
}

void PointLightComponent::Update(IGraphics& a_Graphics)
{
	m_PointLight->position = m_Transform->GetPosition();
}

void PointLightComponent::Draw(IGraphics& a_Graphics){}

void PointLightComponent::InspectorDrawUI()
{

}