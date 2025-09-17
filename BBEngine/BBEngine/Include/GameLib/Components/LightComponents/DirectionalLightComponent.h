#include "GameLib/BBComponent.h"
#include "GameLib/Components/TransformComponent.h"
#include "Lights/DirectionalLight.h"

//Note(Stan): Still have to see if I want to convert these into one file and maybe add a base class.

class DirectionalLightComponent : public BBComponent
{
public:
	DirectionalLightComponent(DirectionalLight a_Spotlight, TransformComponent* a_Transform);
	~DirectionalLightComponent() = default;

	void Update(IGraphics& a_Graphics) override;
	void Draw(IGraphics& a_Graphics) override;

	void InspectorDrawUI() override;

private:
	DirectionalLight m_DirectionalLight;
	TransformComponent* m_Transform;
};