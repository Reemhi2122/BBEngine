#pragma once
#include "BBWin.h"
#include "BBException.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Graphics.h"
#include "Utils/WindowsthrowMacros.h"

#include <memory>
#include <optional>

class WindowClass {
public:
	static const char* GetName() noexcept;
	static HINSTANCE GetInstance() noexcept;

private:
	WindowClass() noexcept;
	~WindowClass();
	WindowClass(const WindowClass&) = delete;
	WindowClass& operator=(const WindowClass&) = delete;
	static constexpr const char* wndClassName = "BBEngine Window";
	static WindowClass wndClass;
	HINSTANCE hInst;
};

class BBWindow {
public:
	BBWindow(int a_Width, int a_Height, const char* a_Name);
	~BBWindow();
	BBWindow(const BBWindow&) = delete;
	BBWindow& operator=(const BBWindow&) = delete;

	void SetTitle(const std::string a_Title);
	static std::optional<int> ProcessMessages();
	Graphics& GetGraphics();

	static LRESULT CALLBACK BBHandleMsgSetup(HWND a_hWnd, UINT a_Msg, WPARAM a_WParam, LPARAM a_LParam) noexcept;
	static LRESULT CALLBACK BBHandleMsgThunk(HWND a_hWnd, UINT a_Msg, WPARAM a_WParam, LPARAM a_LParam) noexcept;
	LRESULT BBHandleMsg(HWND a_hWnd, UINT a_Msg, WPARAM a_WParam, LPARAM a_LParam) noexcept;

public:
	Keyboard m_Keyboard;
	Mouse m_Mouse;

private:
	int m_Width;
	int m_Height;
	HWND m_hWnd;

	Graphics* m_Graphics;
};