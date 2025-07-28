#include "cstmlib/Log.h"
#include "cstmlib/Profiling.h"

int main(int argc, char **argv)
{
    LOG_INIT();
    PROFILER_INIT(0);

    int x = 0;
    const auto res = REP_TEST([&](){std::this_thread::sleep_for(std::chrono::milliseconds(10));}, 100, 100, 100);
    LOG_INFO("Rep Test:\n\n{}\n", std::string(res));

    PROFILER_END();
}
