#pragma once
#include <cstdint>
#include <string>
#include <functional>

#ifndef NOPROFILE
    #define PROFILER_INIT(maxStamps) pll::sProfiler.init(maxStamps)
    #define PROFILER_END() pll::sProfiler.end()

    #define PROFILE_SCOPE_NAMED(name) pll::Timer timer(name)
    #define PROFILE_SCOPE() PROFILE_SCOPE_NAMED(__FUNCTION__)

    #define REP_TEST(func, bytesProcessed, maxTotalReps, maxRepsSinceLastMin) pll::repetitionTest(func, bytesProcessed, maxTotalReps, maxRepsSinceLastMin)
#else
    #define PROFILER_INIT(maxStamps)
    #define PROFILER_END()

    #define PROFILE_SCOPE_NAMED(name)
    #define PROFILE_SCOPE()

    #define REP_TEST(func, bytesProcessed, maxTotalReps, maxRepsSinceLastMin) 
#endif

namespace pll
{
    struct TimeStamp
    {
        uint64_t tscs;
        const char* name;
    };

    inline struct Profiler
    {
        void init(uint32_t maxTimeStamps = 10);
        void end();
        void addTimeStamp(TimeStamp timeStamp);

        TimeStamp* timeStamps{nullptr};
        uint64_t startTime{0};
        uint32_t stampCount{0}, maxStampCount{0};
    } sProfiler;

    class Timer
    {
    public:
        Timer(const char* name);
        ~Timer();
    private:
        const char* mName;
        uint64_t mStartTime{};
    };

    double tscsToMs(uint64_t tscs);

    struct RepetitionResult
    {
        uint64_t minTSCs, maxTSCs, avgTSCs, bytesProcessed;
        uint32_t repetitions, bestRepIndex, worstRepIndex;

        operator std::string() const;
    };

    RepetitionResult repetitionTest(const std::function<void()>& func, uint64_t bytesProcessed = 0, uint32_t maxTotalReps = 512, uint32_t maxRepsSinceLastMin = 128);
}
