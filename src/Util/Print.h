#pragma once
#include <chrono>
#include <iostream>

inline std::string GetCurrentTimeAsString()
{
    auto now = std::chrono::system_clock::now();
    std::time_t time_t = std::chrono::system_clock::to_time_t(now);
    std::tm timeinfo;
    localtime_r(&time_t, &timeinfo);

    return std::format("[{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}] ", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
}

template <typename Stream, typename... Args>
void PrintTo(Stream& stream, Args&&... args) noexcept
{
    stream << GetCurrentTimeAsString();
    ((stream << args), ...) << std::endl;
}

template <typename... Args>
void PrintOut(Args&&... args) noexcept
{
    PrintTo(std::cout, std::forward<Args>(args)...);
}

template <typename... Args>
void PrintErr(Args&&... args) noexcept
{
    PrintTo(std::cerr, std::forward<Args>(args)...);
}
