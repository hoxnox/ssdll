#include "logger.h"

#include <sstream>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef _WIN32
std::string utf8_encode(const std::wstring &wstr) {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}
#endif

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

    Logger &Logger::operator<<(const std::string &s) {
        m_LogLines.push_back(s);
        return *this;
    }

    Logger &Logger::operator<<(const std::wstring &s) {
#ifdef _WIN32
        std::string str = utf8_encode(s);
#else
        std::string str(s.begin(), s.end());
#endif
        m_LogLines.push_back(str);
        return *this;
    }
}
