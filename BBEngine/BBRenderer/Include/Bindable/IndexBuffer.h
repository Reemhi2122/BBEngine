#pragma once
#include "Bindable/Bindable.h"
#include <vector>

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& a_Gfx, std::vector<unsigned short>& a_Indices);
	void Bind(Graphics& a_Gfx) noexcept;
	UINT GetCount() const noexcept;

private:
	UINT m_Count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CPIndexBuffer;
};