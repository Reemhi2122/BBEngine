#include "Bindable.h"

class Texture : public Bindable
{
public:
	Texture(Graphics& a_Gfx);
	void Bind(Graphics& a_Gfx) noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_SamplerState;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Texture;
};