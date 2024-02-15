#include "Drawable/DrawToTexture.h"

DrawToTexture::DrawToTexture(void* BitmapBuffer, VertexShader* a_VertexShader, PixelShader* a_PixelShader)
{

	
	AddBind(a_VertexShader);
	AddBind(a_PixelShader);

}