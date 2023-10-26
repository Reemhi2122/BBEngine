#pragma once
#include "Bindable/Bindable.h"

class VertexShader : public Bindable {
public:
	VertexShader(Graphics& a_Gfx, std::wstring a_Path);
	void Bind(Graphics& a_Gfx) noexcept override;
	ID3DBlob* GetByteCode() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> m_ByteCodeBlob;
};