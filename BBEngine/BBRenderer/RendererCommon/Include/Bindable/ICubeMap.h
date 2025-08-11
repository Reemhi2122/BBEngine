#pragma once
#include "Bindable.h"
#define CUBEMAP_SIZE 6

enum class CubeMapType
{
	TEXTURE,
	DEPTH
};

class ICubeMap : public Bindable
{
public:
	ICubeMap() = default;
	~ICubeMap() = default;

	virtual bool Create(IGraphics& a_Gfx) = 0;
	virtual bool Create(IGraphics& a_Gfx, CubeMapType a_Type, uint32_t a_Resolution = 1024u, char* a_TexturePaths = nullptr) = 0;

	virtual void Bind(IGraphics& a_Gfx) noexcept = 0;
	virtual void UnBind(IGraphics& a_Gfx) noexcept {};
};