#include "screen.h"
#include <windows.h>
#include <ShellScalingApi.h>

#pragma comment(lib, "Shcore.lib")

namespace desktop_ctrl {

static ScreenInfo g_screenInfo = {0, 0, 1.0, 0, 0};

ScreenInfo getScreenInfo() {
    if (g_screenInfo.logicalWidth > 0) {
        return g_screenInfo;
    }

    g_screenInfo.logicalWidth = GetSystemMetrics(SM_CXSCREEN);
    g_screenInfo.logicalHeight = GetSystemMetrics(SM_CYSCREEN);

    HDC hdc = GetDC(NULL);
    if (hdc) {
        g_screenInfo.scaleFactor = static_cast<double>(GetDeviceCaps(hdc, LOGPIXELSX)) / 96.0;
        ReleaseDC(NULL, hdc);
    } else {
        g_screenInfo.scaleFactor = 1.0;
    }

    g_screenInfo.physicalWidth = g_screenInfo.logicalWidth;
    g_screenInfo.physicalHeight = g_screenInfo.logicalHeight;

    return g_screenInfo;
}

} // namespace desktop_ctrl
