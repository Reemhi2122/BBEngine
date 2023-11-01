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

	virtual const std::vector<Bindable*>& GetStaticBinds() const noexcept = 0;

protected:
	void SetIndexBuffer(IndexBuffer* a_Buffer);
	IndexBuffer* GetIndexBuffer() const;

private:
	IndexBuffer* m_IndexBuffer = nullptr;
	std::vector<Bindable*> m_Binds;
};

inline void Drawable::SetIndexBuffer(IndexBuffer* a_Buffer)
{
	m_IndexBuffer = a_Buffer;
}

inline IndexBuffer* Drawable::GetIndexBuffer() const
{
	return m_IndexBuffer;
}
