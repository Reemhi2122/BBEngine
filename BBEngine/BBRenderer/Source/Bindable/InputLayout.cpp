#include "Bindable/InputLayout.h"
#include "Utils/GraphicsThrowMacros.h"

InputLayout::InputLayout(Graphics& a_Gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& a_Desc, ID3DBlob* a_Blob)
{
	INFOMAN;

	GFX_THROW_FAILED(a_Gfx.GetDevice()->CreateInputLayout(
		a_Desc.data(),
		(UINT)a_Desc.size(),
		a_Blob->GetBufferPointer(),
		a_Blob->GetBufferSize(),
		&m_InputLayout));
}

void InputLayout::Bind(Graphics& a_Gfx) noexcept
{
	a_Gfx.GetContext()->IASetInputLayout(m_InputLayout.Get());
}