#define GFX_THROW_FAILED(hrcall) do{ if(FAILED(hr = (hrcall))) throw Graphics::HrException(__LINE__, __FILE__, hr); } while(0)
#define GFX_DEVICE_REMOVED_EXCEPT(hr) do{ throw Graphics::DeviceRemovedException(__LINE__, __FILE__, (hr)); } while(0)

#define INFOMAN(gfx) HRESULT hr