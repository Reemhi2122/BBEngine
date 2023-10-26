#pragma once
#include "Graphics.h"
#include <DirectXMath.h>
#include <vector>

class Bindable;
class IndexBuffer;

class Drawable {
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void Draw(Graphics& a_Gfx) const noexcept;
	virtual void Update(float dt) noexcept = 0;
	void AddBind(Bindable* a_Bind) noexcept;
	void AddIndexBuffer(IndexBuffer* a_Buf) noexcept;

private:
	const IndexBuffer* m_IndexBuffer = nullptr;
	std::vector<Bindable*> m_Binds;

};