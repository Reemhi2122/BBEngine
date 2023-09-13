#define BBWD_EXCEPT(hr) BBWindow::HrException(__LINE__, __FILE__, hr)
#define BBWND_EXCEPT_LAST() BBWindow::HrException(__LINE__, __FILE__, GetLastError())
#define CHWND_NOGFX_EXCEPT() BBWindow::NoGfxException( __LINE__,__FILE__ )