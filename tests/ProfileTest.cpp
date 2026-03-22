#include <gtest/gtest.h>
#include "pll/Profiling.h"

class ProfilerTest : public ::testing::Test
{
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(ProfilerTest, RepetitionTest)
{
    const uint64_t bytesProcessed = 1024;

    auto workFunc = []()
    {
        uint32_t sum = 0;
        for(uint32_t i = 0; i < 1000; ++i)
            sum += i;
        (void)sum;
    };

    const pll::RepetitionResult res = REP_TEST(workFunc, bytesProcessed, 10, 5);

    EXPECT_GT(res.repetitions, 0u);
    EXPECT_EQ(res.bytesProcessed, bytesProcessed);
    EXPECT_GT(res.minTSCs, 0u);
    EXPECT_GE(res.maxTSCs, res.minTSCs);
    EXPECT_GE(res.avgTSCs, res.minTSCs);
    EXPECT_LE(res.avgTSCs, res.maxTSCs);
}

TEST_F(ProfilerTest, ScopeTimer)
{
    EXPECT_EQ(pll::sProfiler.stampCount, 0);

    {
        PROFILE_SCOPE();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    EXPECT_GT(pll::sProfiler.stampCount, 0);
    EXPECT_GT(pll::sProfiler.timeStamps[0].tscs, 0);
}