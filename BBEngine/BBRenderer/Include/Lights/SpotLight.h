#pragma once
#include "BBMath.h"

class SpotLight {
public:
	SpotLight();
	SpotLight(Vector3 a_Position, Vector3 a_Direction, float a_Cone, Vector3 a_Attenuation, Vector4 a_Ambient, Vector4 a_Diffuse, float a_Range);
	~SpotLight();

	//bool GenerateDepthMap(Graphics& a_Gfx);
public:
	Vector3 position;
	float pad0;
	Vector3 direction;
	float cone;
	Vector3 attenuation;
	float range;
	Vector4 ambient;
	Vector4 diffuse;

private:
	//ID3D11DepthStencilView* m_DepthStencilView;
};