#pragma once
#include "Bindable/Bindable.h"
#include <vector>

struct Vertex {
	struct {
		float x, y, z;
	} pos;
};

class VertexBuffer : public Bindable {
public:
	VertexBuffer(Graphics& a_Gfx, const std::vector<Vertex>& a_Vertices);
	void Bind(Graphics& a_Gfx) noexcept override;
	UINT GetCount() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
	UINT m_Count;
};