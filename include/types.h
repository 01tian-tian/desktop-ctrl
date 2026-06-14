#pragma once

#include <string>
#include <optional>

namespace desktop_ctrl {

struct ScreenInfo {
    int logicalWidth;
    int logicalHeight;
    double scaleFactor;
    int physicalWidth;
    int physicalHeight;
};

struct JsonOutput {
    bool ok = true;
    std::string error;
    std::string path;
    std::string data;
    int width = 0;
    int height = 0;
    double scaleFactor = 1.0;
    std::string format;
    std::string version;
    std::string platform;

    std::string toJson() const;
};

std::string getToolDir();
std::string getScreenshotPath(const std::string& format = "bmp");

} // namespace desktop_ctrl
