#include "cstmlib/Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace cstm
{
    std::shared_ptr<spdlog::logger> Log::s_Logger;

    void Log::init(const char* loggerTitle, const spdlog::level::level_enum level, const char* filePath)
    {
        if (filePath)
            s_Logger = spdlog::basic_logger_mt(loggerTitle, filePath);
        else
            s_Logger = spdlog::stdout_color_mt(loggerTitle);
        s_Logger->set_level(level);
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
        s_Logger->set_pattern(fmt);
    }
}
