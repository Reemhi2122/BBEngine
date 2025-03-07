#include "Lights/PointLight.h"

PointLight::PointLight() {}

PointLight::PointLight(Vector3 a_Position, Vector3 a_Attenuation, Vector4 a_Ambient, Vector4 a_Diffuse, float a_Range)
	: position(a_Position), attenuation(a_Attenuation), ambient(a_Ambient), diffuse(a_Diffuse), range(a_Range)
{}

PointLight::~PointLight() {}