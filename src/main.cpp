#include <iostream>
#include <string>
#include <cstring>
#include <windows.h>
#include <ShellScalingApi.h>
#include "types.h"
#include "json.h"
#include "screen.h"
#include "screenshot.h"
#include "input.h"
#include "clipboard.h"

static void printUsage() {
    desktop_ctrl::JsonOutput out;
    out.ok = false;
    out.error = "Usage: desktop-ctrl <command> [args]\n"
                "Commands: screenshot, screen-size, click, double-click, right-click, middle-click,\n"
                "          drag, mouse-move, scroll, type, hotkey, press, release, wait, clipboard, info";
    std::cerr << out.toJson() << std::endl;
}

static std::string getArg(int argc, char* argv[], const std::string& flag, const std::string& def = "") {
    for (int i = 2; i < argc - 1; ++i) {
        if (std::string(argv[i]) == flag) {
            return argv[i + 1];
        }
    }
    return def;
}

static bool hasFlag(int argc, char* argv[], const std::string& flag) {
    for (int i = 2; i < argc; ++i) {
        if (std::string(argv[i]) == flag) return true;
    }
    return false;
}

int main(int argc, char* argv[]) {
    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::string cmd = argv[1];
    desktop_ctrl::JsonOutput out;

    if (cmd == "info") {
        out.ok = true;
        out.version = "1.0.0";
        out.platform = "windows";
        std::cout << out.toJson() << std::endl;
        return 0;
    }

    if (cmd == "screenshot") {
        std::string fmt = getArg(argc, argv, "--format", "png");
        out.width = desktop_ctrl::getScreenInfo().logicalWidth;
        out.height = desktop_ctrl::getScreenInfo().logicalHeight;
        out.scaleFactor = desktop_ctrl::getScreenInfo().scaleFactor;
        out.format = fmt;

        if (desktop_ctrl::takeScreenshot(fmt)) {
            out.path = desktop_ctrl::getScreenshotPath(fmt);
            out.ok = true;
        } else {
            out.ok = false;
            out.error = "Screenshot failed";
        }
        std::cout << out.toJson() << std::endl;
        return out.ok ? 0 : 2;
    }

    if (cmd == "screen-size") {
        desktop_ctrl::ScreenInfo info = desktop_ctrl::getScreenInfo();
        out.ok = true;
        out.width = info.logicalWidth;
        out.height = info.logicalHeight;
        out.scaleFactor = info.scaleFactor;
        std::cout << out.toJson() << std::endl;
        return 0;
    }

    if (cmd == "click") {
        double x = std::stod(getArg(argc, argv, "--x", "0.5"));
        double y = std::stod(getArg(argc, argv, "--y", "0.5"));
        out.ok = desktop_ctrl::mouseClick(x, y);
        if (!out.ok) out.error = "Click failed";
        std::cout << out.toJson() << std::endl;
        return out.ok ? 0 : 2;
    }

    if (cmd == "double-click") {
        double x = std::stod(getArg(argc, argv, "--x", "0.5"));
        double y = std::stod(getArg(argc, argv, "--y", "0.5"));
        out.ok = desktop_ctrl::mouseDoubleClick(x, y);
        if (!out.ok) out.error = "Double-click failed";
        std::cout << out.toJson() << std::endl;
        return out.ok ? 0 : 2;
    }

    if (cmd == "right-click") {
        double x = std::stod(getArg(argc, argv, "--x", "0.5"));
        double y = std::stod(getArg(argc, argv, "--y", "0.5"));
        out.ok = desktop_ctrl::mouseRightClick(x, y);
        if (!out.ok) out.error = "Right-click failed";
        std::cout << out.toJson() << std::endl;
        return out.ok ? 0 : 2;
    }

    if (cmd == "middle-click") {
        double x = std::stod(getArg(argc, argv, "--x", "0.5"));
        double y = std::stod(getArg(argc, argv, "--y", "0.5"));
        out.ok = desktop_ctrl::mouseMiddleClick(x, y);
        if (!out.ok) out.error = "Middle-click failed";
        std::cout << out.toJson() << std::endl;
        return out.ok ? 0 : 2;
    }

    if (cmd == "drag") {
        double sx = std::stod(getArg(argc, argv, "--sx", "0.0"));
        double sy = std::stod(getArg(argc, argv, "--sy", "0.0"));
        double ex = std::stod(getArg(argc, argv, "--ex", "0.5"));
        double ey = std::stod(getArg(argc, argv, "--ey", "0.5"));
        out.ok = desktop_ctrl::mouseDrag(sx, sy, ex, ey);
        if (!out.ok) out.error = "Drag failed";
        std::cout << out.toJson() << std::endl;
        return out.ok ? 0 : 2;
    }

    if (cmd == "mouse-move") {
        double x = std::stod(getArg(argc, argv, "--x", "0.5"));
        double y = std::stod(getArg(argc, argv, "--y", "0.5"));
        out.ok = desktop_ctrl::mouseMove(x, y);
        if (!out.ok) out.error = "Mouse move failed";
        std::cout << out.toJson() << std::endl;
        return out.ok ? 0 : 2;
    }

    if (cmd == "scroll") {
        double x = std::stod(getArg(argc, argv, "--x", "0.5"));
        double y = std::stod(getArg(argc, argv, "--y", "0.5"));
        std::string dir = getArg(argc, argv, "--direction", "down");
        out.ok = desktop_ctrl::mouseScroll(x, y, dir);
        if (!out.ok) out.error = "Scroll failed";
        std::cout << out.toJson() << std::endl;
        return out.ok ? 0 : 2;
    }

    if (cmd == "type") {
        if (hasFlag(argc, argv, "--stdin")) {
            out.ok = desktop_ctrl::typeFromStdin();
        } else {
            std::string text = getArg(argc, argv, "--text", "");
            out.ok = desktop_ctrl::typeText(text);
        }
        if (!out.ok) out.error = "Type failed";
        std::cout << out.toJson() << std::endl;
        return out.ok ? 0 : 2;
    }

    if (cmd == "hotkey") {
        std::string keys = getArg(argc, argv, "--keys", "");
        out.ok = desktop_ctrl::hotkey(keys);
        if (!out.ok) out.error = "Hotkey failed";
        std::cout << out.toJson() << std::endl;
        return out.ok ? 0 : 2;
    }

    if (cmd == "press") {
        std::string key = getArg(argc, argv, "--key", "");
        out.ok = desktop_ctrl::pressKey(key);
        if (!out.ok) out.error = "Press failed";
        std::cout << out.toJson() << std::endl;
        return out.ok ? 0 : 2;
    }

    if (cmd == "release") {
        std::string key = getArg(argc, argv, "--key", "");
        out.ok = desktop_ctrl::releaseKey(key);
        if (!out.ok) out.error = "Release failed";
        std::cout << out.toJson() << std::endl;
        return out.ok ? 0 : 2;
    }

    if (cmd == "wait") {
        int ms = std::stoi(getArg(argc, argv, "--ms", "5000"));
        out.ok = desktop_ctrl::waitMs(ms);
        if (!out.ok) out.error = "Wait failed";
        std::cout << out.toJson() << std::endl;
        return out.ok ? 0 : 2;
    }

    if (cmd == "clipboard") {
        if (hasFlag(argc, argv, "--get")) {
            std::string text = desktop_ctrl::clipboardGet();
            out.ok = true;
            out.data = text;
        } else if (hasFlag(argc, argv, "--set")) {
            std::string text = getArg(argc, argv, "--set", "");
            out.ok = desktop_ctrl::clipboardSet(text);
            if (!out.ok) out.error = "Clipboard set failed";
        } else {
            out.ok = false;
            out.error = "clipboard requires --get or --set";
        }
        std::cout << out.toJson() << std::endl;
        return out.ok ? 0 : 2;
    }

    if (cmd == "cursor-pos") {
        desktop_ctrl::CursorPos pos = desktop_ctrl::getCursorPos();
        out.ok = true;
        out.x = pos.x;
        out.y = pos.y;
        out.normalizedX = pos.normalizedX;
        out.normalizedY = pos.normalizedY;
        out.width = desktop_ctrl::getScreenInfo().logicalWidth;
        out.height = desktop_ctrl::getScreenInfo().logicalHeight;
        std::cout << out.toJson() << std::endl;
        return 0;
    }

    out.ok = false;
    out.error = "Unknown command: " + cmd;
    std::cerr << out.toJson() << std::endl;
    return 1;
}
