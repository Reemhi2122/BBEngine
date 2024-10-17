#include "Lights/SpotLight.h"
#include "Graphics.h"

SpotLight::SpotLight() {}

SpotLight::SpotLight(Vector3 a_Position, Vector3 a_Direction, float a_Cone, Vector3 a_Attenuation, Vector4 a_Ambient, Vector4 a_Diffuse, float a_Range)
	: position(a_Position), direction(a_Direction), cone(a_Cone), attenuation(a_Attenuation), ambient(a_Ambient), diffuse(a_Diffuse), range(a_Range)
{}

SpotLight::~SpotLight() {}

//bool SpotLight::GenerateDepthMap(Graphics& a_Gfx, Camera a_OrgCam)
//{
//    Camera cam;
//    Vector3 focusPoint = position + direction;
//    cam.m_ViewMatrix = DirectX::XMMatrixLookAtLH(
//        DirectX::XMVectorSet(position.x, position.y, position.z, 0),
//        DirectX::XMVectorSet(focusPoint.x, focusPoint.y, focusPoint.z, 1.0f),
//        DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
//    );
//
//    a_Gfx.SetCamera(&cam);
//    a_Gfx.SetDepthStencilTarget();
//
//    for (size_t i = 0; i < a_GameObjects.size(); i++)
//    {
//        GameObject* obj = a_GameObjects[i];
//        obj->GetModel()->SetCurrentShader(a_VSShadowMapShader, a_PSShadowMapShader);
//        obj->Draw(m_Graphics);
//        obj->GetModel()->ResetShaders();
//    }
//
//    a_Gfx.ResetRenderTarget();
//    a_Gfx.SetCamera(&a_OrgCam);
//}