#pragma once
#include "Bindable/Bindable.h"
#include <vector>
#include "DataTypes/SharedRenderTypes.h"

class VertexBuffer : public Bindable {
public:
	VertexBuffer(Graphics& a_Gfx, Vertex* a_Vertices, const uint32_t a_Count);
	void Bind(Graphics& a_Gfx) noexcept override;
	UINT GetCount() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	UINT m_Count;
};