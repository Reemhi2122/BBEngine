#pragma once
#include <DirectXMath.h>

class DirectionalLight
{
public:
	DirectionalLight();
	~DirectionalLight();

private:
	DirectX::XMFLOAT3 m_Dir;
	DirectX::XMFLOAT4 m_Ambient;
	DirectX::XMFLOAT4 m_Diffuse;
	float pad;
};