#pragma once
#include <windows.h>
#include <d3d11.h>
#include <string>
#include <stdio.h>

class BBWinRenderer {

private:

public:
	int RenderInit();
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};