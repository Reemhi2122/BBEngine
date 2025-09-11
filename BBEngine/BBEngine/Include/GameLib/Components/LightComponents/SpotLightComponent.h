#include "GameLib/BBComponent.h"
#include "GameLib/Components/TransformComponent.h"
#include "Lights/SpotLight.h"

//Note(Stan): Still have to see if I want to convert these into one file and maybe add a base class.

class SpotlightComponent : public BBComponent
{
public:
	SpotlightComponent(SpotLight* a_Spotlight, TransformComponent* a_Transform);
	~SpotlightComponent() = default;

	void Update(IGraphics& a_Graphics) override;
	void Draw(IGraphics& a_Graphics) override;

	void InspectorDrawUI() override;

private:
	SpotLight* m_Spotlight;
	TransformComponent* m_Transform;
};