#pragma once
#include "BaseStruct/BaseCandle.h"
#include "Interface/IAnalyzer.h"
#include "Strategy1Detail.h"

class Strategy1Analyzer : public IAnalyzer<Strategy1Analyzer>
{
    using Indicator = Strategy1::Strategy1Indicator;
    using Signal = Strategy1::Strategy1Signal;
    using SignalArg = Strategy1::Strategy1SignalArg;
    using Result = Strategy1::Strategy1Result;

    std::vector<BaseCandle> Candles{};

public:
    void SetCandlesImpl(Symbol symbol, Interval interval) noexcept;
    std::vector<Indicator> MakeIndicatorsImpl() noexcept;
    std::vector<Signal> MakeSignalsImpl(SignalArg&& arg) noexcept;
    std::vector<Result> MakeResultsImpl() noexcept;
};