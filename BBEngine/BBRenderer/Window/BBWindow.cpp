#include "BBWindow.h"
#include <sstream>


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

BBWindow::BBWindow(int a_Width, int a_Height, const char* a_Name){
    RECT wr;
    wr.left = 100;
    wr.right = a_Width + wr.left;
    wr.top = 100;
    wr.bottom = a_Height + wr.top;
    
    if (FAILED(AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE))) {
        throw BBWD_EXCEPT_LAST();
    }

    m_hWnd = CreateWindow(
        WindowClass::GetName(), a_Name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        NULL, NULL, WindowClass::GetInstance(), this
    );

    if (m_hWnd == nullptr) {
        throw BBWD_EXCEPT_LAST();
    }

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
    case WM_SYSKEYDOWN:
        if(!(a_LParam & 0x40000000) || m_Keyboard.AutorepeatIsEnabled()) //Check for repeat presses
            m_Keyboard.OnKeyPressed(static_cast<unsigned char>(a_WParam));
        break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
        m_Keyboard.OnKeyReleased(static_cast<unsigned char>(a_WParam));
        break;    
    case WM_CHAR:
        m_Keyboard.OnChar(static_cast<unsigned char>(a_WParam));
        break;
    case WM_KILLFOCUS:
        m_Keyboard.ClearState();
        break;
    }

    return DefWindowProc(a_hWnd, a_Msg, a_WParam, a_LParam);
}

BBWindow::WindowException::WindowException(int a_Line, const char* a_File, HRESULT a_HR) noexcept 
    : BBException(a_Line, a_File),
      m_HR(a_HR)
{}

const char* BBWindow::WindowException::what() const noexcept{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code]" << GetErrorCode() << std::endl
        << "[Descriptoin]" << GetErrorString() << std::endl
        << GetOriginString() << std::endl;
    m_WhatBuffer = oss.str();
    return m_WhatBuffer.c_str();
}

const char* BBWindow::WindowException::GetType() const noexcept {
    return "BBWindow Exception";
}

std::string BBWindow::WindowException::TranslateErrorCode(HRESULT a_HR) noexcept {
    char* pMsgBuf = nullptr;
    DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, a_HR, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);

    if (nMsgLen == 0)
        return "Unidentified error code";
    
    std::string errorString = pMsgBuf;
    LocalFree(pMsgBuf);
    return errorString;
}

HRESULT BBWindow::WindowException::GetErrorCode() const noexcept {
    return m_HR;
}

std::string BBWindow::WindowException::GetErrorString() const noexcept {
    return TranslateErrorCode(m_HR);
}