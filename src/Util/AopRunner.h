#pragma once
#include "Constant.h"
#include "Macro.h"
#include "Print.h"
#include "Type.h"

#include <chrono>
#include <functional>
#include <type_traits>

inline int& GetMaxCycleWidth()
{
    static int max_width = 10;
    return max_width;
}

template <typename Func, typename... Args>
auto MeasureCycles(Func&& func, Args&&... args)
{
    auto start = Capture;
    auto time_start = std::chrono::high_resolution_clock::now();

    if constexpr (std::is_void_v<std::invoke_result_t<Func, Args...>>)
    {
        std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
        auto end = Capture;
        auto time_end = std::chrono::high_resolution_clock::now();
        u64 cycles = end - start;
        auto ms = std::chrono::duration<double, std::milli>(time_end - time_start).count();

        int width = std::to_string(cycles).length();
        GetMaxCycleWidth() = std::max(GetMaxCycleWidth(), width);

        PrintOut(std::format("{:>{}} cycles, {}ms", cycles, GetMaxCycleWidth(), ms));
    }
    else
    {
        decltype(auto) result = std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
        auto end = Capture;
        auto time_end = std::chrono::high_resolution_clock::now();

        u64 cycles = end - start;
        auto ms = std::chrono::duration<double, std::milli>(time_end - time_start).count();
        int width = std::to_string(cycles).length();
        GetMaxCycleWidth() = std::max(GetMaxCycleWidth(), width);

        PrintOut(std::format("{:>{}} cycles, {}ms", cycles, GetMaxCycleWidth(), ms));
        return result;
    }
}
