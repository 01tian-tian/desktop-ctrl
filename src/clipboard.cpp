#include "clipboard.h"
#include <windows.h>
#include <string>

namespace desktop_ctrl {

std::string clipboardGet() {
    if (!OpenClipboard(NULL)) return "";

    std::string result;
    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (hData) {
        wchar_t* pText = (wchar_t*)GlobalLock(hData);
        if (pText) {
            int size = WideCharToMultiByte(CP_UTF8, 0, pText, -1, NULL, 0, NULL, NULL);
            if (size > 0) {
                std::string utf8(size - 1, 0);
                WideCharToMultiByte(CP_UTF8, 0, pText, -1, &utf8[0], size, NULL, NULL);
                result = utf8;
            }
            GlobalUnlock(hData);
        }
    }

    CloseClipboard();
    return result;
}

bool clipboardSet(const std::string& text) {
    if (!OpenClipboard(NULL)) return false;

    EmptyClipboard();

    int wsize = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, NULL, 0);
    if (wsize <= 0) {
        CloseClipboard();
        return false;
    }

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, wsize * sizeof(wchar_t));
    if (!hMem) {
        CloseClipboard();
        return false;
    }

    wchar_t* pMem = (wchar_t*)GlobalLock(hMem);
    MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, pMem, wsize);
    GlobalUnlock(hMem);

    SetClipboardData(CF_UNICODETEXT, hMem);
    CloseClipboard();

    return true;
}

} // namespace desktop_ctrl
