#include "Lights/SpotLight.h"

SpotLight::SpotLight() {}

SpotLight::SpotLight(Vector3 a_Position, Vector3 a_Direction, float a_Cone, Vector3 a_Attenuation, Vector4 a_Ambient, Vector4 a_Diffuse, float a_Range)
	: position(a_Position), direction(a_Direction), cone(a_Cone), attenuation(a_Attenuation), ambient(a_Ambient), diffuse(a_Diffuse), range(a_Range)
{}

SpotLight::~SpotLight() {}