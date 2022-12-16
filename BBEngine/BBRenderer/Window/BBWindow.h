#pragma once
#include "../BBWin.h"

class BBWindow {
private:
	class WindowClass {
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;

	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass& ) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "BBEngine Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

public:
	BBWindow(int a_Width, int a_Height, const char* a_Name) noexcept;
	~BBWindow();
	BBWindow(const BBWindow&) = delete;
	BBWindow& operator=(const BBWindow&) = delete;

private:
	static LRESULT CALLBACK BBHandleMsgSetup(HWND a_hWnd, UINT a_Msg, WPARAM a_WParam, LPARAM a_LParam) noexcept;
	static LRESULT CALLBACK BBHandleMsgThunk(HWND a_hWnd, UINT a_Msg, WPARAM a_WParam, LPARAM a_LParam) noexcept;
	LRESULT BBHandleMsg(HWND a_hWnd, UINT a_Msg, WPARAM a_WParam, LPARAM a_LParam) noexcept;

	int m_Width;
	int m_Height;
	HWND m_hWnd;
};