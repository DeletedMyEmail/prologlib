#include "pll/Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace pll
{
    std::shared_ptr<spdlog::logger> Log::sLogger;

    void Log::init(const char* loggerTitle, const spdlog::level::level_enum level, const char* filePath)
    {
        if (filePath)
            sLogger = spdlog::basic_logger_mt(loggerTitle, filePath);
        else
            sLogger = spdlog::stdout_color_mt(loggerTitle);
        sLogger->set_level(level);
        setPattern(NAME | TIME | LEVEL);
    }

    void Log::setPattern(const uint8_t pattern)
    {
        std::string fmt = "";
        if (pattern & PATTERN_TYPE::TIME)
            fmt = "[%H:%M:%S]";
        if (pattern & PATTERN_TYPE::NAME)
            fmt += " [%n]";
        if (pattern & PATTERN_TYPE::THREAD)
            fmt += " [%t]";
        if (pattern & PATTERN_TYPE::LEVEL)
            fmt += " [%l]";

        fmt += ": %v%$";
        sLogger->set_pattern(fmt);
    }
}
