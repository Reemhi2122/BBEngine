#include "Drawable/DrawToTexture.h"

DrawToTexture::DrawToTexture(Graphics& a_Graphics, VertexShader* a_VertexShader, PixelShader* a_PixelShader)
{
    ID3D11Texture2D* texture;
    D3D11_TEXTURE2D_DESC tex_desc;
    tex_desc.Format = DXGI_FORMAT_A8_UNORM;
    tex_desc.Width = 1024;
    tex_desc.Height = 1024;
    tex_desc.MipLevels = 1;
    tex_desc.ArraySize = 1;
    tex_desc.SampleDesc.Count = 1;
    tex_desc.SampleDesc.Quality = 0;
    tex_desc.Usage = D3D11_USAGE_DEFAULT;
    tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET;
    tex_desc.CPUAccessFlags = 0;
    tex_desc.MiscFlags = 0;

    a_Graphics.GetDevice()->CreateTexture2D(&tex_desc, nullptr, &texture);

    AddBind(a_VertexShader);
    AddBind(a_PixelShader);
	
    ID3D11RenderTargetView* renderTarget;
    D3D11_RENDER_TARGET_VIEW_DESC render_desc = {};
    render_desc.Format = DXGI_FORMAT_A8_UNORM;
    render_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    render_desc.Texture2D.MipSlice = 0;

    a_Graphics.GetDevice()->CreateRenderTargetView(texture, &render_desc, &renderTarget);

    a_Graphics.GetContext()->OMSetRenderTargets(1, &renderTarget, nullptr);

    float test = 1.f;
    a_Graphics.GetContext()->ClearRenderTargetView(renderTarget, &test);

    a_Graphics.GetContext()->DrawIndexed(0, 0, 0);

    a_Graphics.ResetRenderTarget();
}

void DrawToTexture::Update(float a_DeltaTime) noexcept 
{

}

DirectX::XMMATRIX DrawToTexture::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixIdentity();
}