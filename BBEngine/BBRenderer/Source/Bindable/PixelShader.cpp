#include "Bindable/PixelShader.h"
#include <d3dcompiler.h>

PixelShader::PixelShader(Graphics& a_Gfx, const std::wstring& a_Path)
{
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	D3DCompileFromFile(a_Path.c_str(), nullptr, nullptr, "main", "ps_5_0", D3DCOMPILE_WARNINGS_ARE_ERRORS, 0, &blob, nullptr);
	a_Gfx.GetDevice()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_PixelShader);
}

void PixelShader::Bind(Graphics& a_Gfx) noexcept
{
	a_Gfx.GetContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0);
}