#include "BBRenderer.h"

int BBWinRenderer::RenderInit()
{
    try {
        BBWindow wnd(800, 300, "BBWindow test");

        MSG msg;
        BOOL gResult;
        while ((gResult = GetMessage(&msg, NULL, 0, 0)) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (wnd.m_Keyboard.KeyIsPressed(VK_SPACE)) {
                MessageBox(nullptr, "Space bar pressed!", "Space bar was pressed", 0);
            }
        }

        if (gResult == -1) {
            return -1;
        }

        return msg.wParam;
    }
    catch (const BBException& e) {
        MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e) {
        MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...) {
        MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    return -1;
}

