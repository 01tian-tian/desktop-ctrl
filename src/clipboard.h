#pragma once

#include <string>

namespace desktop_ctrl {

std::string clipboardGet();
bool clipboardSet(const std::string& text);

} // namespace desktop_ctrl
