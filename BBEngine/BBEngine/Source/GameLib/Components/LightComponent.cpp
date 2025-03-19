#include "GameLib/BBComponent.h"

class LightComponent : public BBComponent
{
public:
	LightComponent();
	~LightComponent();

	void Update(Graphics& a_Graphics) override;
	void Draw(Graphics& a_Graphics) override;

	void InspectorDrawUI() override;

private:

};