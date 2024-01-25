#include "Bindable.h"

class Texture : public Bindable
{
public:
	Texture(Graphics& a_Gfx);


private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CubesTexture;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> CubesTexSamplerState;
};