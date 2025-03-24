#include "GameLib/Components/SpotLightComponent.h"

SpotlightComponent::SpotlightComponent(SpotLight* a_Spotlight, Transform* a_Transform)
{
	m_Spotlight = a_Spotlight;
	m_Transform = a_Transform;
}

void SpotlightComponent::Update(Graphics& a_Graphics)
{
	m_Spotlight->position = m_Transform->GetPosition();
}

void SpotlightComponent::Draw(Graphics& a_Graphics)
{}

void SpotlightComponent::InspectorDrawUI()
{
		
}