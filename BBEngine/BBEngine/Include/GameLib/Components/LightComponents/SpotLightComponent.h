#include "GameLib/BBComponent.h"
#include "GameLib/Components/Transform.h"
#include "Lights/SpotLight.h"

//Note(Stan): Still have to see if I want to convert these into one file and maybe add a base class.

class SpotlightComponent : public BBComponent
{
public:
	SpotlightComponent(SpotLight* a_Spotlight, Transform* a_Transform);
	~SpotlightComponent() = default;

	void Update(Graphics& a_Graphics) override;
	void Draw(Graphics& a_Graphics) override;

	void InspectorDrawUI() override;

private:
	SpotLight* m_Spotlight;
	Transform* m_Transform;
};