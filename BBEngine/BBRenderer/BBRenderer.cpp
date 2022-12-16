#include "BBRenderer.h"

int BBWinRenderer::RenderInit()
{
    BBWindow wnd(800, 300, "BBWindow test");

    MSG msg;
    BOOL gResult;
    while ((gResult = GetMessage(&msg, NULL, 0, 0)) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (gResult == -1) {
        return -1;
    }

    return msg.wParam;
}

