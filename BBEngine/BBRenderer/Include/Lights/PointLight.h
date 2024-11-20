#pragma once
#include "BBMath.h"
#include "Bindable/CubeMap.h"

class PointLight {
public:
	PointLight();
	PointLight(Vector3 a_Position, Vector3 a_Attenuation, Vector4 a_Ambient, Vector4 a_Diffuse, float a_Range);
	~PointLight();

public:
	Vector3 position;
	float pad0;
	Vector3 attenuation;
	float range;
	Vector4 ambient;
	Vector4 diffuse;

	CubeMap* DepthStencilCubeMap;
};