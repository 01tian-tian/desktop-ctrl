#include "types.h"
#include "json.h"
#include <windows.h>
#include <string>

namespace desktop_ctrl {

static std::wstring getToolDirW() {
    wchar_t buf[MAX_PATH] = {0};
    GetModuleFileNameW(NULL, buf, MAX_PATH);
    std::wstring path(buf);
    size_t pos = path.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        std::wstring exeDir = path.substr(0, pos);
        size_t scriptsPos = exeDir.find_last_of(L"\\/");
        if (scriptsPos != std::wstring::npos) {
            std::wstring parentDir = exeDir.substr(0, scriptsPos);
            std::wstring folderName = exeDir.substr(scriptsPos + 1);
            if (folderName == L"scripts") {
                return parentDir;
            }
        }
        return exeDir;
    }
    return L".";
}

std::string getToolDir() {
    std::wstring wdir = getToolDirW();
    int len = WideCharToMultiByte(CP_UTF8, 0, wdir.c_str(), (int)wdir.size(), nullptr, 0, nullptr, nullptr);
    if (len <= 0) return ".";
    std::string dir(len, 0);
    WideCharToMultiByte(CP_UTF8, 0, wdir.c_str(), (int)wdir.size(), &dir[0], len, nullptr, nullptr);
    return dir;
}

std::string getScreenshotPath(const std::string& format) {
    std::wstring wdir = getToolDirW() + L"\\Temp";

    DWORD attrib = GetFileAttributesW(wdir.c_str());
    if (attrib == INVALID_FILE_ATTRIBUTES) {
        CreateDirectoryW(wdir.c_str(), NULL);
    }

    int len = WideCharToMultiByte(CP_UTF8, 0, wdir.c_str(), (int)wdir.size(), nullptr, 0, nullptr, nullptr);
    std::string dir(len, 0);
    WideCharToMultiByte(CP_UTF8, 0, wdir.c_str(), (int)wdir.size(), &dir[0], len, nullptr, nullptr);

    return dir + "\\screenshot." + format;
}

std::string JsonOutput::toJson() const {
    std::ostringstream oss;
    oss << "{\"ok\":" << (ok ? "true" : "false");

    if (!error.empty()) {
        oss << ",\"error\":\"" << escapeJsonString(error) << "\"";
    }
    if (!path.empty()) {
        oss << ",\"path\":\"" << escapeJsonString(path) << "\"";
    }
    if (!data.empty()) {
        oss << ",\"data\":\"" << escapeJsonString(data) << "\"";
    }
    if (width > 0) {
        oss << ",\"width\":" << width;
    }
    if (height > 0) {
        oss << ",\"height\":" << height;
    }
    if (scaleFactor > 0.0) {
        oss << ",\"scaleFactor\":" << scaleFactor;
    }
    if (!format.empty()) {
        oss << ",\"format\":\"" << format << "\"";
    }
    if (!version.empty()) {
        oss << ",\"version\":\"" << version << "\"";
    }
    if (!platform.empty()) {
        oss << ",\"platform\":\"" << platform << "\"";
    }

    oss << "}";
    return oss.str();
}

} // namespace desktop_ctrl
