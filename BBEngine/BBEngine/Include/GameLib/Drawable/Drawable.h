#pragma once
#include "IGraphics.h"
#include <DirectXMath.h>
#include <vector>
#include "BBMath.h"

#include "Bindable/Bindable.h"
#include "Bindable/IIndexBuffer.h"

class Drawable {
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;

	void SetPosition(Vector3 a_Position) { };
	virtual Vector3 GetPosition() const { return Vector3(); };
	
	virtual void Draw(IGraphics& a_Gfx) noexcept;
	virtual void DrawInstanced(IGraphics& a_Gfx, uint32_t a_InstanceCount) noexcept;
	virtual void Update(float dt) noexcept = 0;

	virtual void BindShaders() {};
	
	void AddBind(Bindable* a_Bind) noexcept;
	//void AddIndexBuffer(IndexBuffer* a_Buf) noexcept;

	//virtual const std::vector<Bindable*>& GetStaticBinds() const noexcept { return std::vector<Bindable*>(); };

protected:
	IIndexBuffer* GetIndexBuffer() const;
	void SetIndexBuffer(IIndexBuffer* a_Buffer);

private:
	IIndexBuffer* m_IndexBuffer = nullptr;
	std::vector<Bindable*> m_Binds;
};

inline void Drawable::SetIndexBuffer(IIndexBuffer* a_Buffer)
{
	m_IndexBuffer = a_Buffer;
}

inline IIndexBuffer* Drawable::GetIndexBuffer() const
{
	return m_IndexBuffer;
}
