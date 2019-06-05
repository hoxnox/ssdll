#include "logger.h"

#include <sstream>

namespace ssdll {
    std::function<void(const std::string&)> Logger::s_Callback;

    Logger::~Logger() {
        if (!s_Callback) { return; }

        std::stringstream ss;

        for (auto &s: m_LogLines) {
            ss << s << ' ';
        }

        Logger::s_Callback(ss.str());
    }
}
