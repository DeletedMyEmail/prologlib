#include "pll/Log.h"
#include "pll/Profiling.h"
#include <gtest/gtest.h>

int main(int argc, char **argv)
{
    LOG_INIT_SPECIFIC("Test", spdlog::level::info, nullptr);
    PROFILER_INIT(10);

    ::testing::InitGoogleTest(&argc, argv);
    const int res = RUN_ALL_TESTS();

    PROFILER_END();
    return res;
}
