#include "Bindable/VertexShader.h"
#include <d3dcompiler.h>

VertexShader::VertexShader(Graphics& a_Gfx, std::wstring a_Path)
{
	D3DCompileFromFile(a_Path.c_str(), nullptr, nullptr, "main", "vs_5_0", 0, 0, &m_ByteCodeBlob, nullptr);
	a_Gfx.GetDevice()->CreateVertexShader(m_ByteCodeBlob->GetBufferPointer(), m_ByteCodeBlob->GetBufferSize(), nullptr, &m_VertexShader);
}

void VertexShader::Bind(Graphics& a_Gfx) noexcept
{
	a_Gfx.GetContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0);
}

ID3DBlob* VertexShader::GetByteCode() const noexcept
{
	return m_ByteCodeBlob.Get();
}
