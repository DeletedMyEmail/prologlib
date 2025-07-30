#pragma once

#include <spdlog/spdlog.h>

#ifndef NOLOG
    #define LOG_INIT_SPECIFIC(title, lvl, filepath) cstm::Log::init(title, lvl, filepath)
    #define LOG_INIT() cstm::Log::init(PROJECT_NAME)
    #define SET_PATTERN(x) cstm::Log::setPattern(x)

    #define LOG_WARN(x...) cstm::Log::getLogger()->warn(fmt::format(x))
    #define LOG_INFO(x...) cstm::Log::getLogger()->info(fmt::format(x))
    #define LOG_ERROR(x...) cstm::Log::getLogger()->error(fmt::format(x))
#else
    #define LOG_INIT_SPECIFIC(title, lvl, filepath)
    #define LOG_INIT()
    #define SET_PATTERN(x)

    #define LOG_WARN(x...)
    #define LOG_INFO(x...)
    #define LOG_ERROR(x...)
#endif

namespace cstm
{
    class Log
    {
    public:
        static void init(const char* loggerTitle = "App", spdlog::level::level_enum level = spdlog::level::trace, const char* filePath = nullptr);
        static std::shared_ptr<spdlog::logger>& getLogger() { return s_Logger; }
        static void setPattern(uint8_t pattern);

        enum PATTERN_TYPE : uint8_t
        {
            TIME = 0b1,
            THREAD = 0b10,
            LEVEL = 0b100,
            NAME = 0b1000,
        };
    private:
        static std::shared_ptr<spdlog::logger> s_Logger;
    };

}
