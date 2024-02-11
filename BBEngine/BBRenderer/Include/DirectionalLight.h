#pragma once
#include "Vector3.h"
#include "Vector4.h"

class DirectionalLight
{
public:
	DirectionalLight();
	DirectionalLight(Vector3 a_Direction, Vector4 a_Ambient, Vector4 a_Diffuse, float a_Pad);
	~DirectionalLight();

private:
	Vector3 direction;
	Vector4 ambient;
	Vector4 diffuse;
	float pad;
};