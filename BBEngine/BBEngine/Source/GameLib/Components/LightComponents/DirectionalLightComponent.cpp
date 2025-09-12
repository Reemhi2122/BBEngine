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

void DirectionalLightComponent::Draw(IGraphics& a_Graphics) 
{
    float cx = -20.0f, cy = 50.0f, cz = 0.0f;
	Vector3 FakePos = Vector3(cx, cy, cz);
	Vector3 focusPoint = Vector3(0, 0, 0);

	float length = sqrt(cx * cx + cy * cy + cz * cz);
	m_DirectionalLight.direction = Vector3(cx / length, cy / length, cz / length);

	DirectX::XMMATRIX lightView = DirectX::XMMatrixLookAtLH(
	    DirectX::XMVectorSet(FakePos.x, FakePos.y, FakePos.z, 0),
	    DirectX::XMVectorSet(focusPoint.x, focusPoint.y, focusPoint.z, 1.0f),
	    DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	);

	DirectX::XMMATRIX projection = DirectX::XMMatrixOrthographicLH(150.f, 150.f, 0.5f, 500.f);
	DirectX::XMMATRIX oldProjection = m_Cam2.GetProjection();
	m_Cam2.SetProjection(projection);
	m_Cam2.SetViewPort(8192, 8192);

	m_DirectionalLight.lightView = DirectX::XMMatrixTranspose(lightView * projection);

	m_Cam2.m_ViewMatrix = lightView;
	m_Graphics.SetCamera(&m_Cam2);
	m_Graphics.SetDepthStencilTarget(m_DLTextureDepthStencilView);
	
	for (size_t i = 0; i < a_GameObjects.size(); i++)
	{
	    BBObject* obj = a_GameObjects[i];
	    m_Graphics.BindShader(ShaderType::VertexShader, a_VSShadowMapShader);
	    m_Graphics.BindShader(ShaderType::PixelShader, a_PSShadowMapShader);
	    obj->Draw(m_Graphics);
	}

	m_Cam2.SetProjection(oldProjection);
	m_Cam2.SetViewPort(1024, 1024);
	m_Graphics.SetGameViewRenderTarget();
	m_Graphics.SetCamera(&m_Cam1);
}

void DirectionalLightComponent::InspectorDrawUI()
{

}