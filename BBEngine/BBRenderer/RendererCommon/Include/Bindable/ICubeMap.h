#include "Bindable.h"

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

	virtual bool Create(IGraphics& a_Gfx);
	virtual bool Create(IGraphics& a_Gfx, CubeMapType a_Type, uint32_t a_Resolution = 1024u, char* a_TexturePaths = nullptr);

	virtual void Bind(IGraphics& a_Gfx) noexcept = 0;
	virtual void UnBind(IGraphics& a_Gfx) noexcept {};
};