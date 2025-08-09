#pragma once

#if 0

#include "Drawable/DrawableBase.h"
#include <random>
#include "Bindable/BindableInclude.h"

struct ConstantBufferColor {
	struct {
		float r;
		float g;
		float b;
		float a;
	}face_colors[6];
};

class Box : public DrawableBase<Box> {

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

	Sampler*		m_Sampler;
	PixelConstantBuffer<ConstantBufferColor>* cCB;
};

#endif