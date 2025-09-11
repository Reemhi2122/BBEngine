#include "GameLib/Components/LightComponents/SpotLightComponent.h"

SpotlightComponent::SpotlightComponent(SpotLight* a_Spotlight, TransformComponent* a_Transform)
{
	m_Spotlight = a_Spotlight;
	m_Transform = a_Transform;
}

void SpotlightComponent::Update(IGraphics& a_Graphics)
{
	m_Spotlight->position = m_Transform->GetPosition();
}

void SpotlightComponent::Draw(IGraphics& a_Graphics)
{
	
}

void SpotlightComponent::InspectorDrawUI()
{
		
}