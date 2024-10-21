#pragma once
#include "Drawable/DrawableBase.h"
#include "Bindable/BindableInclude.h"

class Box : public DrawableBase<Box>
{
public:
	Box(Graphics& a_Gfx);

	void Update(float a_DeltaTime) noexcept override;
	void Draw(Graphics& a_Gfx) noexcept override;

private:
	VertexBuffer*	vBuffer;
	IndexBuffer*	IBuffer;
	uint32_t		vShader;
	uint32_t		pShader;
	InputLayout*	m_InputLayout;
	Topology*		m_Topology;
	TransformBuf*	m_TransformBuf;
	CubeMap*		m_CubeMap;
	Sampler*		m_Sampler;
};