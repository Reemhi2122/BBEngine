#pragma once
#include "BBWin.h"
#include "BBException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Utils/WindowsthrowMacros.h"

#include <memory>
#include <optional>

class BBWindow {
public:
	class Exception : public BBException {
	public:
		using BBException::BBException;
		static std::string TranslateErrorCode(HRESULT a_Hr) noexcept;
	};

	class HrException : public Exception {
	public:
		HrException(int a_Line, const char* a_File, HRESULT a_HR) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;

	private:
		HRESULT m_Hr;
	};

	class NoGfxException : public Exception {
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
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
	Graphics& GetGraphics();

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

	std::unique_ptr<Graphics> m_Graphics;
};