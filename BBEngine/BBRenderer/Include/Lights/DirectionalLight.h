#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include <DirectXMath.h>

class DirectionalLight
{
public:
	DirectionalLight();
	DirectionalLight(Vector3 a_Direction, Vector4 a_Ambient, Vector4 a_Diffuse);
	~DirectionalLight();

private:
	Vector3 direction;
	float pad0;
	Vector4 ambient;
	Vector4 diffuse;
	DirectX::XMMATRIX lightView;
};