#include "Graphics.h"

#include "combaseapi.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

Graphics::Graphics(HWND a_HWnd)
{

}

bool Graphics::Initialize()
{
	HRESULT hres;

	IDXGIFactory4* dxgiFactory;
	hres = CreateDXGIFactory1(IDD_PPV_ARGS(&dxgiFactory));

	return true;
}