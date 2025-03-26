#include "Lights/DirectionalLight.h"

DirectionalLight::DirectionalLight()
{}

DirectionalLight::DirectionalLight(Vector3 a_Direction, Vector4 a_Ambient, Vector4 a_Diffuse)
	: direction(a_Direction), ambient(a_Ambient), diffuse(a_Diffuse)
{}

DirectionalLight::~DirectionalLight()
{}
