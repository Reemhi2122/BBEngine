#include "BBWindow.h"


BBWindow::WindowClass BBWindow::WindowClass::wndClass;

BBWindow::WindowClass::WindowClass() noexcept 
	: hInst(GetModuleHandle(nullptr)) {

    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = BBHandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = NULL;
    wc.hCursor = NULL;
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.hIconSm = NULL;
    wc.lpszClassName = GetName();
    RegisterClassEx(&wc);
}

BBWindow::WindowClass::~WindowClass() {
    UnregisterClass(wndClassName, GetInstance());
}

const char* BBWindow::WindowClass::GetName() noexcept {
    return wndClassName;
}

HINSTANCE BBWindow::WindowClass::GetInstance() noexcept {
    return wndClass.hInst;
}

BBWindow::BBWindow(int a_Width, int a_Height, const char* a_Name) noexcept{
    RECT wr;
    wr.left = 100;
    wr.right = a_Width + wr.left;
    wr.top = 100;
    wr.bottom = a_Height + wr.top;
    AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    m_hWnd = CreateWindow(
        WindowClass::GetName(), a_Name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        NULL, NULL, WindowClass::GetInstance(), this
    );

    ShowWindow(m_hWnd, SW_SHOWDEFAULT);
}

BBWindow::~BBWindow() {
    DestroyWindow(m_hWnd);
}

LRESULT CALLBACK BBWindow::BBHandleMsgSetup(HWND a_hWnd, UINT a_Msg, WPARAM a_WParam, LPARAM a_LParam) noexcept
{
    if (a_Msg == WM_NCCREATE) {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(a_LParam);
        BBWindow* const pWnd = static_cast<BBWindow*>(pCreate->lpCreateParams);
        SetWindowLongPtr(a_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        SetWindowLongPtr(a_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&BBWindow::BBHandleMsgThunk));
        return pWnd->BBHandleMsg(a_hWnd, a_Msg, a_WParam, a_LParam);
    }

    return DefWindowProc(a_hWnd, a_Msg, a_WParam, a_LParam);
}

LRESULT CALLBACK BBWindow::BBHandleMsgThunk(HWND a_hWnd, UINT a_Msg, WPARAM a_WParam, LPARAM a_LParam) noexcept
{
    BBWindow* const pWnd = reinterpret_cast<BBWindow*>(GetWindowLongPtr(a_hWnd, GWLP_USERDATA));
    return pWnd->BBHandleMsg( a_hWnd, a_Msg, a_WParam, a_LParam);
}

LRESULT BBWindow::BBHandleMsg(HWND a_hWnd, UINT a_Msg, WPARAM a_WParam, LPARAM a_LParam) noexcept
{
    switch (a_Msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(a_hWnd, &ps);

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(a_hWnd, &ps);
    }
    break;
    case WM_KEYDOWN:
        switch (a_WParam)
        {
        case 'F':
            SetWindowText(a_hWnd, "TestFKey");
        }
        break;
    }

    return DefWindowProc(a_hWnd, a_Msg, a_WParam, a_LParam);
}