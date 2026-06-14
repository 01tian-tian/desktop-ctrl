#include "screenshot.h"
#include "screen.h"
#include "types.h"
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <iostream>

#pragma comment(lib, "gdiplus.lib")

namespace desktop_ctrl {

class GdiplusToken {
public:
    GdiplusToken() {
        Gdiplus::GdiplusStartupInput input;
        Gdiplus::GdiplusStartup(&token, &input, NULL);
    }
    ~GdiplusToken() {
        Gdiplus::GdiplusShutdown(token);
    }
private:
    ULONG_PTR token;
};

static void drawCursor(HDC hdc) {
    CURSORINFO ci = {0};
    ci.cbSize = sizeof(CURSORINFO);
    if (!GetCursorInfo(&ci)) return;
    if (!(ci.flags & CURSOR_SHOWING)) return;

    ICONINFO ii = {0};
    if (!GetIconInfo(ci.hCursor, &ii)) return;

    int x = ci.ptScreenPos.x - ii.xHotspot;
    int y = ci.ptScreenPos.y - ii.yHotspot;

    DrawIconEx(hdc, x, y, ci.hCursor, 0, 0, 0, NULL, DI_NORMAL);

    if (ii.hbmMask) DeleteObject(ii.hbmMask);
    if (ii.hbmColor) DeleteObject(ii.hbmColor);
}

static std::wstring utf8ToWide(const std::string& utf8) {
    if (utf8.empty()) return {};
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), (int)utf8.size(), nullptr, 0);
    if (len <= 0) return {};
    std::wstring wstr(len, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), (int)utf8.size(), &wstr[0], len);
    return wstr;
}

static CLSID getEncoderClsid(const std::wstring& format) {
    UINT num = 0, size = 0;
    Gdiplus::GetImageEncodersSize(&num, &size);
    if (size == 0) return {};

    std::vector<BYTE> buf(size);
    auto* encoders = reinterpret_cast<Gdiplus::ImageCodecInfo*>(buf.data());
    Gdiplus::GetImageEncoders(num, size, encoders);

    for (UINT i = 0; i < num; ++i) {
        if (encoders[i].MimeType == format) {
            return encoders[i].Clsid;
        }
    }
    return {};
}

bool takeScreenshot(const std::string& format) {
    ScreenInfo info = getScreenInfo();

    HDC hScreenDC = GetDC(NULL);
    if (!hScreenDC) return false;

    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
    if (!hMemoryDC) {
        ReleaseDC(NULL, hScreenDC);
        return false;
    }

    int width = info.physicalWidth;
    int height = info.physicalHeight;

    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);
    if (!hBitmap) {
        DeleteDC(hMemoryDC);
        ReleaseDC(NULL, hScreenDC);
        return false;
    }

    HGDIOBJ hOldBitmap = SelectObject(hMemoryDC, hBitmap);

    BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);

    drawCursor(hMemoryDC);

    SelectObject(hMemoryDC, hOldBitmap);

    GdiplusToken gdiplusToken;

    std::wstring mimeType = (format == "png") ? L"image/png" : L"image/bmp";
    CLSID clsid = getEncoderClsid(mimeType);

    Gdiplus::Bitmap bitmap(hBitmap, NULL);
    
    std::string path = getScreenshotPath(format);
    std::wstring wpath = utf8ToWide(path);

    Gdiplus::Status status = bitmap.Save(wpath.c_str(), &clsid, NULL);

    DeleteObject(hBitmap);
    DeleteDC(hMemoryDC);
    ReleaseDC(NULL, hScreenDC);

    return status == Gdiplus::Ok;
}

} // namespace desktop_ctrl
