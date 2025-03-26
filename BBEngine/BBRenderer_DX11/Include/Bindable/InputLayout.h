#pragma once
#include "Bindable/Bindable.h"
#include <vector>

class InputLayout : public Bindable {
public:
	InputLayout(Graphics& a_Gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& a_Desc, ID3DBlob* a_Blob);
	void Bind(Graphics& a_Gfx) noexcept override;

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
};