#pragma once
#include "../BBWin.h"
#include "../BBException/BBException.h"
#include "../Input/Keyboard.h"
#include "../Input/Mouse.h"

#include <optional>

class BBWindow {
public:
	class WindowException : public BBException {
	public:
		WindowException(int a_Line, const char* a_File, HRESULT a_HR) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT a_HR) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;

	private:
		HRESULT m_HR;
	};

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
	BBWindow(int a_Width, int a_Height, const char* a_Name);
	~BBWindow();
	BBWindow(const BBWindow&) = delete;
	BBWindow& operator=(const BBWindow&) = delete;
	void SetTitle(const std::string a_Title);
	static std::optional<int> ProcessMessages();

private:
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
};

#define BBWD_EXCEPT(hr) BBWindow::WindowException(__LINE__, __FILE__, hr)
#define BBWND_EXCEPT_LAST() BBWindow::WindowException(__LINE__, __FILE__, GetLastError())