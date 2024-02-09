#pragma once
#include <DirectXMath.h>

class DirectionalLight
{
public:
	DirectionalLight();
	~DirectionalLight();

private:
	DirectX::XMFLOAT3 dir;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	float pad;
};