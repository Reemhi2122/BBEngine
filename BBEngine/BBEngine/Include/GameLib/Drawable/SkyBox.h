#pragma once
#include "GameLib/Drawable/Drawable.h"
#include "GFXInclude/BBGFXPlatform.h"

#include "Bindable/IVertexBuffer.h"
#include "Bindable/IIndexBuffer.h"
#include "GameLib/TransformBuf.h"
#include "Bindable/ICubeMap.h"

class Skybox : public Drawable
{
public:
	Skybox(IGraphics& a_Gfx);

	void Update(float a_DeltaTime) noexcept override;
	void Draw(IGraphics& a_Gfx) noexcept override;

private:
	IVertexBuffer*	vBuffer;
	IIndexBuffer*	IBuffer;
	uint32_t		vShader;
	uint32_t		pShader;
	//InputLayout*	m_InputLayout;
	//Topology*		m_Topology;
	TransformBuf*	m_TransformBuf;
	ICubeMap*		m_CubeMap;
	//Sampler*		m_Sampler;
};