#include "Drawable/DrawToTexture.h"

DrawToTexture::DrawToTexture(Graphics& a_Graphics, VertexShader* a_VertexShader, PixelShader* a_PixelShader)
{
	

	AddBind(a_VertexShader);
	AddBind(a_PixelShader);
}

void DrawToTexture::Update(float a_DeltaTime) noexcept 
{

}

DirectX::XMMATRIX DrawToTexture::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixIdentity();
}