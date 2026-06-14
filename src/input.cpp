#include "input.h"
#include "screen.h"
#include <windows.h>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>

#ifndef MOUSEEVENTF_HWHEEL
#define MOUSEEVENTF_HWHEEL 0x01000
#endif

namespace desktop_ctrl {

static INPUT createKeyEvent(WORD vk, bool keyUp) {
    INPUT input = {};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vk;
    input.ki.dwFlags = keyUp ? KEYEVENTF_KEYUP : 0;
    return input;
}

static int vkFromName(const std::string& name) {
    std::string lower = name;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if (lower == "ctrl" || lower == "control") return VK_CONTROL;
    if (lower == "alt") return VK_MENU;
    if (lower == "shift") return VK_SHIFT;
    if (lower == "win" || lower == "meta" || lower == "command" || lower == "cmd") return VK_LWIN;
    if (lower == "enter" || lower == "return") return VK_RETURN;
    if (lower == "tab") return VK_TAB;
    if (lower == "esc" || lower == "escape") return VK_ESCAPE;
    if (lower == "space") return VK_SPACE;
    if (lower == "backspace") return VK_BACK;
    if (lower == "delete" || lower == "del") return VK_DELETE;
    if (lower == "home") return VK_HOME;
    if (lower == "end") return VK_END;
    if (lower == "pageup" || lower == "page up") return VK_PRIOR;
    if (lower == "pagedown" || lower == "page down") return VK_NEXT;
    if (lower == "up" || lower == "arrowup") return VK_UP;
    if (lower == "down" || lower == "arrowdown") return VK_DOWN;
    if (lower == "left" || lower == "arrowleft") return VK_LEFT;
    if (lower == "right" || lower == "arrowright") return VK_RIGHT;
    if (lower == "f1") return VK_F1;
    if (lower == "f2") return VK_F2;
    if (lower == "f3") return VK_F3;
    if (lower == "f4") return VK_F4;
    if (lower == "f5") return VK_F5;
    if (lower == "f6") return VK_F6;
    if (lower == "f7") return VK_F7;
    if (lower == "f8") return VK_F8;
    if (lower == "f9") return VK_F9;
    if (lower == "f10") return VK_F10;
    if (lower == "f11") return VK_F11;
    if (lower == "f12") return VK_F12;
    if (lower == "printscreen") return VK_SNAPSHOT;
    if (lower == "capslock") return VK_CAPITAL;
    if (lower == "numlock") return VK_NUMLOCK;
    if (lower == "scrolllock") return VK_SCROLL;

    if (lower.length() == 1) {
        char c = lower[0];
        if (c >= 'a' && c <= 'z') return c - 'a' + 'A';
        if (c >= '0' && c <= '9') return c - '0' + 0x30;
        switch (c) {
            case ';': return VK_OEM_1;
            case '=': return VK_OEM_PLUS;
            case ',': return VK_OEM_COMMA;
            case '-': return VK_OEM_MINUS;
            case '.': return VK_OEM_PERIOD;
            case '/': return VK_OEM_2;
            case '`': return VK_OEM_3;
            case '[': return VK_OEM_4;
            case '\\': return VK_OEM_5;
            case ']': return VK_OEM_6;
            case '\'': return VK_OEM_7;
        }
    }

    return 0;
}

static std::vector<std::string> splitKeys(const std::string& keys) {
    std::vector<std::string> result;
    std::istringstream iss(keys);
    std::string token;
    while (std::getline(iss, token, '+')) {
        while (!token.empty() && token[0] == ' ') token.erase(0, 1);
        while (!token.empty() && token.back() == ' ') token.pop_back();
        if (!token.empty()) result.push_back(token);
    }
    if (result.empty()) {
        std::istringstream iss2(keys);
        while (std::getline(iss2, token, ' ')) {
            if (!token.empty()) result.push_back(token);
        }
    }
    return result;
}

static POINT normalizedToPixel(double x, double y) {
    ScreenInfo info = getScreenInfo();
    POINT pt;
    pt.x = static_cast<LONG>(x * info.logicalWidth);
    pt.y = static_cast<LONG>(y * info.logicalHeight);
    return pt;
}

bool mouseClick(double x, double y) {
    POINT pt = normalizedToPixel(x, y);
    SetCursorPos(pt.x, pt.y);
    Sleep(50);

    INPUT inputs[2] = {};
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dx = 0;
    inputs[0].mi.dy = 0;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dx = 0;
    inputs[1].mi.dy = 0;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    return SendInput(2, inputs, sizeof(INPUT)) == 2;
}

bool mouseDoubleClick(double x, double y) {
    POINT pt = normalizedToPixel(x, y);
    SetCursorPos(pt.x, pt.y);
    Sleep(50);

    INPUT inputs[4] = {};
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    inputs[2].type = INPUT_MOUSE;
    inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    inputs[3].type = INPUT_MOUSE;
    inputs[3].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    return SendInput(4, inputs, sizeof(INPUT)) == 4;
}

bool mouseRightClick(double x, double y) {
    POINT pt = normalizedToPixel(x, y);
    SetCursorPos(pt.x, pt.y);
    Sleep(50);

    INPUT inputs[2] = {};
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;

    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;

    return SendInput(2, inputs, sizeof(INPUT)) == 2;
}

bool mouseMiddleClick(double x, double y) {
    POINT pt = normalizedToPixel(x, y);
    SetCursorPos(pt.x, pt.y);
    Sleep(50);

    INPUT inputs[2] = {};
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;

    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;

    return SendInput(2, inputs, sizeof(INPUT)) == 2;
}

bool mouseDrag(double sx, double sy, double ex, double ey) {
    POINT start = normalizedToPixel(sx, sy);
    POINT end = normalizedToPixel(ex, ey);

    SetCursorPos(start.x, start.y);
    Sleep(50);

    INPUT inputs[3] = {};

    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    int steps = 20;
    for (int i = 1; i <= steps; ++i) {
        LONG cx = start.x + (end.x - start.x) * i / steps;
        LONG cy = start.y + (end.y - start.y) * i / steps;

        inputs[1].type = INPUT_MOUSE;
        inputs[1].mi.dx = cx;
        inputs[1].mi.dy = cy;
        inputs[1].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK;
        SendInput(1, &inputs[1], sizeof(INPUT));
        Sleep(10);
    }

    inputs[2].type = INPUT_MOUSE;
    inputs[2].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(1, &inputs[0], sizeof(INPUT));
    SendInput(1, &inputs[2], sizeof(INPUT));

    return true;
}

bool mouseMove(double x, double y) {
    POINT pt = normalizedToPixel(x, y);
    return SetCursorPos(pt.x, pt.y) != 0;
}

bool mouseScroll(double x, double y, const std::string& direction) {
    POINT pt = normalizedToPixel(x, y);
    SetCursorPos(pt.x, pt.y);
    Sleep(50);

    int delta = 0;
    std::string dir = direction;
    std::transform(dir.begin(), dir.end(), dir.begin(), ::tolower);

    if (dir == "up") delta = 120 * 5;
    else if (dir == "down") delta = -120 * 5;
    else if (dir == "left") delta = 120 * 5;
    else if (dir == "right") delta = -120 * 5;
    else return false;

    INPUT input = {};
    input.type = INPUT_MOUSE;

    if (dir == "left" || dir == "right") {
        input.mi.dwFlags = MOUSEEVENTF_HWHEEL;
    } else {
        input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    }
    input.mi.mouseData = delta;

    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool typeText(const std::string& text) {
    std::string content = text;
    if (content.size() >= 2 && content.substr(content.size() - 2) == "\\n") {
        content = content.substr(0, content.size() - 2);
    }
    if (!content.empty() && content.back() == '\n') {
        content.pop_back();
    }

    if (content.empty()) {
        INPUT enter = createKeyEvent(VK_RETURN, false);
        INPUT enterUp = createKeyEvent(VK_RETURN, true);
        SendInput(1, &enter, sizeof(INPUT));
        SendInput(1, &enterUp, sizeof(INPUT));
        return true;
    }

    // Save original clipboard
    std::wstring originalClipboard;
    if (OpenClipboard(NULL)) {
        HANDLE hData = GetClipboardData(CF_UNICODETEXT);
        if (hData) {
            wchar_t* pData = (wchar_t*)GlobalLock(hData);
            if (pData) {
                originalClipboard = pData;
                GlobalUnlock(hData);
            }
        }
        CloseClipboard();
    }

    // Convert ANSI to wide string (system default code page)
    int wideLen = MultiByteToWideChar(CP_ACP, 0, content.c_str(), -1, NULL, 0);
    if (wideLen > 0) {
        std::wstring wideContent(wideLen, 0);
        MultiByteToWideChar(CP_ACP, 0, content.c_str(), -1, &wideContent[0], wideLen);
        wideContent.pop_back(); // Remove null terminator

        if (OpenClipboard(NULL)) {
            EmptyClipboard();
            HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (wideContent.size() + 1) * sizeof(wchar_t));
            if (hMem) {
                wchar_t* pMem = (wchar_t*)GlobalLock(hMem);
                wcscpy_s(pMem, wideContent.size() + 1, wideContent.c_str());
                GlobalUnlock(hMem);
                SetClipboardData(CF_UNICODETEXT, hMem);
            }
            CloseClipboard();
        }
    }

    // Paste with Ctrl+V
    INPUT inputs[4] = {};
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;
    
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 'V';
    
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 'V';
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
    
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(4, inputs, sizeof(INPUT));
    Sleep(50);

    // Restore original clipboard
    if (!originalClipboard.empty()) {
        if (OpenClipboard(NULL)) {
            EmptyClipboard();
            HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (originalClipboard.size() + 1) * sizeof(wchar_t));
            if (hMem) {
                wchar_t* pMem = (wchar_t*)GlobalLock(hMem);
                wcscpy_s(pMem, originalClipboard.size() + 1, originalClipboard.c_str());
                GlobalUnlock(hMem);
                SetClipboardData(CF_UNICODETEXT, hMem);
            }
            CloseClipboard();
        }
    }

    if (!text.empty() && text.back() == '\n') {
        INPUT enter = createKeyEvent(VK_RETURN, false);
        INPUT enterUp = createKeyEvent(VK_RETURN, true);
        SendInput(1, &enter, sizeof(INPUT));
        SendInput(1, &enterUp, sizeof(INPUT));
    }

    return true;
}

bool typeFromStdin() {
    std::string text;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (!text.empty()) text += "\n";
        text += line;
    }
    return typeText(text);
}

bool hotkey(const std::string& keys) {
    auto keyNames = splitKeys(keys);
    if (keyNames.empty()) return false;

    std::vector<INPUT> inputs;
    std::vector<WORD> vks;

    for (const auto& name : keyNames) {
        int vk = vkFromName(name);
        if (vk == 0) return false;
        vks.push_back(static_cast<WORD>(vk));
    }

    for (WORD vk : vks) {
        inputs.push_back(createKeyEvent(vk, false));
    }
    for (auto it = vks.rbegin(); it != vks.rend(); ++it) {
        inputs.push_back(createKeyEvent(*it, true));
    }

    return SendInput(static_cast<UINT>(inputs.size()), inputs.data(), sizeof(INPUT)) == inputs.size();
}

bool pressKey(const std::string& key) {
    int vk = vkFromName(key);
    if (vk == 0) return false;

    INPUT input = createKeyEvent(static_cast<WORD>(vk), false);
    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool releaseKey(const std::string& key) {
    int vk = vkFromName(key);
    if (vk == 0) return false;

    INPUT input = createKeyEvent(static_cast<WORD>(vk), true);
    return SendInput(1, &input, sizeof(INPUT)) == 1;
}

bool waitMs(int ms) {
    Sleep(ms);
    return true;
}

} // namespace desktop_ctrl
