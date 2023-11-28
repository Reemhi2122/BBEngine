#include "BBWindow.h"
#include <sstream>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

WindowClass WindowClass::wndClass;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

WindowClass::WindowClass() noexcept 
	: hInst(GetModuleHandle(nullptr)) {

    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = BBWindow::BBHandleMsgSetup;
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

WindowClass::~WindowClass() {
    UnregisterClass(wndClassName, GetInstance());
}

const char* WindowClass::GetName() noexcept {
    return wndClassName;
}

HINSTANCE WindowClass::GetInstance() noexcept {
    return wndClass.hInst;
}

BBWindow::BBWindow(int a_Width, int a_Height, const char* a_Name)
: m_Width(a_Width), m_Height(a_Height) {
    RECT wr;
    wr.left = 100;
    wr.right = a_Width + wr.left;
    wr.top = 100;
    wr.bottom = a_Height + wr.top;
    
    if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0) {
        throw BBWND_EXCEPT_LAST();
    }

    m_hWnd = CreateWindow(
        WindowClass::GetName(), a_Name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, WindowClass::GetInstance(), this
    );

    if (m_hWnd == nullptr) {
        throw BBWND_EXCEPT_LAST();
    }

    ShowWindow(m_hWnd, SW_SHOWDEFAULT);

    m_Graphics = std::make_unique<Graphics>(m_hWnd);
}

BBWindow::~BBWindow() {
    DestroyWindow(m_hWnd);
}

void BBWindow::SetTitle(const std::string a_Title) {
    if (SetWindowText(m_hWnd, a_Title.c_str()) == 0) {
        throw BBWND_EXCEPT_LAST();
    }
}

std::optional<int> BBWindow::ProcessMessages() {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

        if (msg.message == WM_QUIT)
            return msg.wParam;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return {};
}

Graphics& BBWindow::GetGraphics() {
    if (!m_Graphics)
        throw CHWND_NOGFX_EXCEPT();
    return *m_Graphics;
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
    if (ImGui_ImplWin32_WndProcHandler(a_hWnd, a_Msg, a_WParam, a_LParam))
        return true;

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

    // Keyboard
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

    //Mouse
    case WM_MOUSEMOVE:
        const POINTS pt = MAKEPOINTS(a_LParam);
        if (pt.x >= 0 && pt.x < m_Width && pt.y > 0 && pt.y < m_Height) {
            m_Mouse.OnMouseMove(pt.x, pt.y);
            if (!m_Mouse.IsInWindow()) {
                SetCapture(m_hWnd);
                m_Mouse.OnMouseEnter();
            }
        } else {
            if (a_WParam & (MK_LBUTTON | MK_RBUTTON)) {
                m_Mouse.OnMouseMove(pt.x, pt.y);
            } else {
                ReleaseCapture();
                m_Mouse.OnMouseLeave();
            }
        }
        break;
    case WM_LBUTTONDOWN:
        m_Mouse.OnLeftPressed(0, 0);
        break;
    case WM_LBUTTONUP:
        m_Mouse.OnLeftReleased(0, 0);
        break;
    case WM_RBUTTONDOWN:
        m_Mouse.OnRightPressed(0,0);
        break;
    case WM_RBUTTONUP:
        m_Mouse.OnRightReleased(0, 0);
        break;
    case WM_MOUSEWHEEL:
        {
            const int delta = GET_WHEEL_DELTA_WPARAM(a_WParam);
            m_Mouse.OnWheelDelta(0, 0, delta);
        }
        break;

    //Break focus
    case WM_KILLFOCUS:
        m_Keyboard.ClearState();
        break;
    }

    return DefWindowProc(a_hWnd, a_Msg, a_WParam, a_LParam);
}

std::string BBWindow::Exception::TranslateErrorCode(HRESULT a_Hr) noexcept
{
    char* pMsgBuf = nullptr;
    DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, a_Hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);

    if (nMsgLen == 0)
        return "Unidentified error code";

    std::string errorString = pMsgBuf;
    LocalFree(pMsgBuf);
    return errorString;
}

BBWindow::HrException::HrException(int a_Line, const char* a_File, HRESULT a_HR) noexcept
    :   Exception(a_Line, a_File),
        m_Hr(a_HR)
{}

const char* BBWindow::HrException::what() const noexcept{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code]" << GetErrorCode() << std::endl
        << "[Descriptoin]" << GetErrorString() << std::endl
        << GetOriginString() << std::endl;
    m_WhatBuffer = oss.str();
    return m_WhatBuffer.c_str();
}

const char* BBWindow::HrException::GetType() const noexcept {
    return "[BBWindow Exception]";
}

HRESULT BBWindow::HrException::GetErrorCode() const noexcept {
    return m_Hr;
}

std::string BBWindow::HrException::GetErrorString() const noexcept {
    return TranslateErrorCode(m_Hr);
}

const char* BBWindow::NoGfxException::GetType() const noexcept
{
    return "[BB Exception [No Graphics]]";
}