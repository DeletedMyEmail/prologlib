#pragma once
#include <spdlog/spdlog.h>

#ifndef NOLOG
    #define LOG_INIT_SPECIFIC(title, lvl, filepath) pll::Log::init(title, lvl, filepath)
    #define LOG_INIT() pll::Log::init(PROJECT_NAME)
    #define SET_PATTERN(x) pll::Log::setPattern(x)

    #define LOG_WARN(x...) pll::Log::getLogger()->warn(fmt::format(x))
    #define LOG_INFO(x...) pll::Log::getLogger()->info(fmt::format(x))
    #define LOG_ERROR(x...) pll::Log::getLogger()->error(fmt::format(x))
#else
    #define LOG_INIT_SPECIFIC(title, lvl, filepath)
    #define LOG_INIT()
    #define SET_PATTERN(x)

    #define LOG_WARN(x...)
    #define LOG_INFO(x...)
    #define LOG_ERROR(x...)
#endif

namespace pll
{
    class Log
    {
    public:
        static void init(const char* loggerTitle = "App", spdlog::level::level_enum level = spdlog::level::trace, const char* filePath = nullptr);
        static std::shared_ptr<spdlog::logger>& getLogger() { return sLogger; }
        static void setPattern(uint8_t pattern);

        enum PATTERN_TYPE : uint8_t
        {
            TIME = 0b1,
            THREAD = 0b10,
            LEVEL = 0b100,
            NAME = 0b1000,
        };
    private:
        static std::shared_ptr<spdlog::logger> sLogger;
    };

}
