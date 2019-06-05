#ifndef LOGGER_H
#define LOGGER_H

#include <functional>
#include <string>
#include <vector>

#define LOG ssdll::Logger()

namespace ssdll {
    struct Logger {
        Logger() {}

        ~Logger();

        Logger &operator<<(const std::string &s);
        Logger &operator<<(const std::wstring &s);

        static std::function<void(const std::string&)> s_Callback;

    private:
        std::vector<std::string> m_LogLines;
    };
}

#endif // LOGGER_H
