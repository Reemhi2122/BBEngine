#include "GameLib/Drawable/SkyBox.h"
#include "Utils/GraphicsThrowMacros.h"
#include "SharedRenderTypes.h"
#include "Graphics.h"

Skybox::Skybox(IGraphics& a_Gfx)
{
	BBE::Vertex vertices[] = {
		{ {-1.0f, -1.0f, -1.0f}, {0, 0}, {0, 0, 0} },
		{ {1.0f, -1.0f, -1.0f}, {0, 0}, {0, 0, 0} },
		{ {-1.0f, 1.0f, -1.0f}, {0, 0}, {0, 0, 0} },
		{ {1.0f, 1.0f, -1.0f}, {0, 0}, {0, 0, 0} },
		{ {-1.0f, -1.0f, 1.0f}, {0, 0}, {0, 0, 0} },
		{ {1.0f, -1.0f, 1.0f}, {0, 0}, {0, 0, 0} },
		{ {-1.0f, 1.0f, 1.0f}, {0, 0}, {0, 0, 0} },
		{ {1.0f, 1.0f, 1.0f}, {0, 0}, {0, 0, 0} }
	};

	vBuffer = GFXCreateVertexBuffer();
	vBuffer->Create(a_Gfx, vertices, sizeof(BBE::Vertex), 8);
	AddBind(vBuffer);

	uint16_t indices[] = {
		1,2,0, 1,3,2,
		5,3,1, 5,7,3,
		3,6,2, 7,6,3,
		7,5,4, 6,7,4,
		2,4,0, 6,4,2,
		4,1,0, 4,5,1
	};

	IBuffer = GFXCreateIndexBuffer();
	IBuffer->Create(a_Gfx, (uint8_t*)indices, 36, 1);
	AddBind(IBuffer);
	SetIndexBuffer(IBuffer);
	
	m_CubeMap = GFXCreateCubeMap();
	m_CubeMap->Create(a_Gfx);
	AddBind(m_CubeMap);

	m_TransformBuf = new TransformBuf(a_Gfx, Vector3(0, 0, 0), Vector4(0, 0, 0, 1), Vector3(2, 2, 2));
	AddBind(m_TransformBuf);
}

void Skybox::Update(float a_DeltaTime) noexcept {};

void Skybox::Draw(IGraphics& a_Gfx) noexcept
{
	a_Gfx.SetGraphicsContext("cubeMap");
	Drawable::Draw(a_Gfx);
	a_Gfx.SetGraphicsContext("main");
}