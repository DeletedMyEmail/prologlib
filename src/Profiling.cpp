#include "cstmlib/Profiling.h"
#include "cstmlib/Log.h"
#include <cassert>
#include <sstream>
#include <x86intrin.h>
#include <sys/time.h>

static double toTotalPercent(uint64_t part, uint64_t total);
static uint64_t estimateCPUFreq(uint64_t testPeriod = 100);
static std::string throughputToString(double throughput);

namespace cstm
{
    TimeStamp* Profiler::s_TimeStamps = nullptr;
    uint32_t Profiler::s_StampCount = 0;
    uint32_t Profiler::s_MaxStampCount = 0;
    uint64_t Profiler::s_StartTime = 0;

    RepetitionResult repetitionTest(const std::function<void()>& func, const uint64_t bytesProcessed, const uint32_t maxTotalReps, const uint32_t maxRepsSinceLastMin)
    {
        RepetitionResult res{};
        res.minTSCs = std::numeric_limits<uint64_t>::max();
        res.bytesProcessed = bytesProcessed;

        uint32_t repSinceMin = 0;

        while (res.repetitions < maxTotalReps && repSinceMin < maxRepsSinceLastMin)
        {
            const uint64_t start = __rdtsc();
            func();
            const uint64_t end = __rdtsc();
            const uint64_t elapsedTSCs = end - start;

            res.repetitions++;
            if (elapsedTSCs > res.avgTSCs)
                res.avgTSCs += (elapsedTSCs - res.avgTSCs) / res.repetitions;
            else
                res.avgTSCs -= (res.avgTSCs - elapsedTSCs) / res.repetitions;

            if (elapsedTSCs > res.maxTSCs)
            {
                res.maxTSCs = elapsedTSCs;
                res.worstRepIndex = res.repetitions;
                repSinceMin++;
            }
            else if (elapsedTSCs < res.minTSCs)
            {
                res.minTSCs = elapsedTSCs;
                repSinceMin = 0;
                res.bestRepIndex = res.repetitions;
            }
            else
                repSinceMin++;
        }

        return res;
    }

    RepetitionResult::operator std::string() const
    {
        const double maxThroughput = static_cast<double>(bytesProcessed) / tscsToMs(minTSCs) * 1000.0;
        const double avgThroughput = static_cast<double>(bytesProcessed) / tscsToMs(avgTSCs) * 1000.0;
        const double minThroughput = static_cast<double>(bytesProcessed) / tscsToMs(maxTSCs) * 1000.0;

        return std::format(
            "Best (at rep {}):\t {} tscs / {:.4f} ms / {} throughput\n"
            "Avg:\t\t\t\t {} tscs / {:.4f} ms / {} throughput\n"
            "Worst (at rep {}):\t {} tscs / {:.4f} ms / {} throughput",
            bestRepIndex, minTSCs, tscsToMs(minTSCs), throughputToString(maxThroughput),
            avgTSCs, tscsToMs(avgTSCs), throughputToString(avgThroughput),
            worstRepIndex, maxTSCs, tscsToMs(maxTSCs), throughputToString(minThroughput));
    }

    void Profiler::init(const uint32_t maxTimeStamps)
    {
        assert(s_TimeStamps == nullptr);

        s_MaxStampCount = maxTimeStamps;
        s_TimeStamps = new TimeStamp[maxTimeStamps];
        s_StartTime = __rdtsc();
    }

    void Profiler::end()
    {
        assert(s_StartTime != 0);
        const uint64_t programTime = __rdtsc() - s_StartTime;

        std::string result = std::format("------------ Profiler results ------------\nTotal Time: {} tscs / {:.4f} ms", programTime, tscsToMs(programTime));

        for(uint32_t i = 0; i < s_StampCount; ++i)
        {
            const auto& [tscs, name] = s_TimeStamps[i];
            result += std::format("Time for {}: {} tscs / {:.4f} ms / {:.4f}% of total\n", name, tscs, tscsToMs(tscs), toTotalPercent(tscs, programTime));
        }

        delete[] s_TimeStamps;
        LOG_INFO("{}", result);
    }

    void Profiler::addTimeStamp(const TimeStamp timeStamp)
    {
        assert(s_StampCount < s_MaxStampCount);
        s_TimeStamps[s_StampCount++] = timeStamp;
    }

    Timer::Timer(const char* name)
        : m_Name(name), m_StartTime(__rdtsc())
    {
    }

    Timer::~Timer()
    {
        const uint64_t endTime = __rdtsc();
        Profiler::addTimeStamp({endTime - m_StartTime, m_Name});
    }

    double tscsToMs(const uint64_t tscs)
    {
        static uint64_t CPUFreq = estimateCPUFreq();

        return static_cast<double>(tscs) * 1000.0 / static_cast<double>(CPUFreq);
    }
}

static std::string throughputToString(double throughput)
{
    uint8_t iterations = 0;
    static std::array<const char*, 5> units = {"b/s", "kb/s", "mb/s", "gb/s", "tb/s"};

    while (throughput > 1024.0 && iterations < units.size())
    {
        throughput /= 1024.0;
        iterations++;
    }

    return std::format("{:.4f} {}", throughput, units[iterations]);
}

static double toTotalPercent(const uint64_t part, const uint64_t total)
{
    if (total == 0)
        return 0.0;

    return (static_cast<double>(part) / static_cast<double>(total)) * 100.0;
}

static uint64_t readOSTime()
{
    timeval val{};
    gettimeofday(&val, nullptr);

    return 1000000 * static_cast<uint64_t>(val.tv_sec) + static_cast<uint64_t>(val.tv_usec);
}

static uint64_t estimateCPUFreq(const uint64_t testPeriod)
{
    const uint64_t freq = 1000000;
    const uint64_t CPUStart = __rdtsc();
    const uint64_t OSStart = readOSTime();
    const uint64_t OSWaitTime = freq * testPeriod / 1000;

    uint64_t OSEnd = 0;
    uint64_t OSElapsed = 0;
    while(OSElapsed < OSWaitTime)
    {
        OSEnd = readOSTime();
        OSElapsed = OSEnd - OSStart;
    }

    const uint64_t CPUEnd = __rdtsc();
    const uint64_t CPUElapsed = CPUEnd - CPUStart;
    const uint64_t CPUFreq = freq * CPUElapsed / OSElapsed;

    return CPUFreq;
}
