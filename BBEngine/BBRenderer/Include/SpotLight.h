#pragma once
#include "BBMath.h"

class SpotLight {
public:
	SpotLight();
	SpotLight(Vector3 a_Position, Vector3 a_Attenuation, Vector4 a_Ambient, Vector4 a_Diffuse, float a_Range);
	~SpotLight();

private:
	Vector3 position;
	Vector3 attenuation;
	Vector4 ambient;
	Vector4 diffuse;

	float range;
};