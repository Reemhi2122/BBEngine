#define BBWD_EXCEPT(hr) HrException(__LINE__, __FILE__, hr)
#define BBWND_EXCEPT_LAST() HrException(__LINE__, __FILE__, GetLastError())
#define CHWND_NOGFX_EXCEPT() NoGfxException( __LINE__,__FILE__ )