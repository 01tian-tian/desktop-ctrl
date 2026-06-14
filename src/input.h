#pragma once

#include <string>

namespace desktop_ctrl {

bool mouseClick(double x, double y);
bool mouseDoubleClick(double x, double y);
bool mouseRightClick(double x, double y);
bool mouseMiddleClick(double x, double y);
bool mouseDrag(double sx, double sy, double ex, double ey);
bool mouseMove(double x, double y);
bool mouseScroll(double x, double y, const std::string& direction);
bool typeText(const std::string& text);
bool typeFromStdin();
bool hotkey(const std::string& keys);
bool pressKey(const std::string& key);
bool releaseKey(const std::string& key);
bool waitMs(int ms);

struct CursorPos {
    long x;
    long y;
    double normalizedX;
    double normalizedY;
};

CursorPos getCursorPos();

} // namespace desktop_ctrl
