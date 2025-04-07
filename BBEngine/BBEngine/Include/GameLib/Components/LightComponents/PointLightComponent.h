#include "GameLib/BBComponent.h"
#include "GameLib/Components/Transform.h"
#include "Lights/PointLight.h"

//Note(Stan): Still have to see if I want to convert these into one file and maybe add a base class.

class PointLightComponent : public BBComponent
{
public:
	PointLightComponent(PointLight* a_Spotlight, Transform* a_Transform);
	~PointLightComponent() = default;

	void Update(Graphics& a_Graphics) override;
	void Draw(Graphics& a_Graphics) override;

	void InspectorDrawUI() override;

private:
	PointLight* m_PointLight;
	Transform* m_Transform;
};