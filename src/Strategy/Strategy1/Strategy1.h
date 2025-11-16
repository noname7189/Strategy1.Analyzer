#pragma once
#include "BaseStruct/BaseCandle.h"
#include "Interface/IAnalyzer.h"
#include "Memory/CacheLineAllocator.h"
#include "Strategy1Detail.h"
#include "Strategy1Helper.h"

class Strategy1 : public IAnalyzer<Strategy1, Strategy1Indicator, Strategy1Signal, Strategy1Result, CacheLineAllocator<Strategy1Indicator>>
{
    using EthCandle = BaseCandle<2>;

    std::vector<EthCandle> Candles{};
    std::vector<Strategy1Indicator, CacheLineAllocator<Strategy1Indicator>> Indicators{};
    u32 SignalStartIndex;

public:
    void SetCandlesImpl(Symbol symbol, Interval interval) noexcept;

    void Ready() noexcept
    {
        Indicators = ReadyIndicators();
        SignalStartIndex = 119;
    }

    void SetIndicatorsImpl(Strategy1IndicatorArg arg) noexcept
    {
        SetIndicators(Indicators, arg);
    }

    FORCE_INLINE u32 GetCandleSizeImpl() noexcept
    {
        return Candles.size();
    }

    FORCE_INLINE Strategy1Indicator MakeIndicator(u32 index, const Strategy1IndicatorArg& arg) noexcept
    {
        constexpr auto null = Decimal<4>::null();

        Strategy1Indicator indicator;
        indicator.CandleIndex = index;

        // constexpr Decimal<4> k5{3333};
        // constexpr Decimal<4> k20{952};
        // constexpr Decimal<4> k60{333};
        // constexpr Decimal<4> k120{165};

        const auto close = Candles[index].Close;

        indicator.EMA1 = likely(index + 1 >= arg.Period1) ? CalculateEMA(arg.KValue1, close, Indicators[index - 1].EMA1) : null;
        indicator.EMA2 = likely(index + 1 >= arg.Period2) ? CalculateEMA(arg.KValue2, close, Indicators[index - 1].EMA2) : null;
        indicator.EMA3 = likely(index + 1 >= arg.Period3) ? CalculateEMA(arg.KValue3, close, Indicators[index - 1].EMA3) : null;
        indicator.EMA4 = likely(index + 1 >= arg.Period4) ? CalculateEMA(arg.KValue4, close, Indicators[index - 1].EMA4) : null;

        return indicator;
    }

    FORCE_INLINE Strategy1Signal MakeSignal(u32 index, const Strategy1SignalArg& arg) noexcept
    {
        Strategy1Signal signal;
        signal.CandleIndex = NAV<u32>();

        auto prevIndicator = Indicators[index - 1];
        auto indicator = Indicators[index];

        if (prevIndicator.EMA1 < prevIndicator.EMA2)
        {
            // EMA1이 EMA2를 추월했을 때만

            if (indicator.EMA1 > indicator.EMA2 && indicator.EMA2 > indicator.EMA3 && indicator.EMA3 > indicator.EMA4)
            {
                auto takeProfitPrice = Candles[index].Close + arg.coeff1 * Candles[index].Open + arg.coeff2 * Candles[index].High + arg.coeff3 * Candles[index].Low;
                auto losscutPrice = Candles[index].Close - (arg.coeff4 * Candles[index].Low).rescale<2>();

                signal.CandleIndex = index;
                signal.TakeProfitPrice = takeProfitPrice.rescale<2>();
                signal.LosscutPrice = losscutPrice;
                signal.Position = SignalPosition::Long;
            }
        }

        return signal;
    };

    FORCE_INLINE Strategy1Result MakeResult(const Strategy1Signal& signal) noexcept
    {
        Strategy1Result result;
        result.StartCandleIndex = signal.CandleIndex;
        result.EndCandleIndex = NAV<u32>();
        result.EnterPrice = Candles[result.StartCandleIndex].Close;

        bool takeProfit = false;
        for (u32 i = signal.CandleIndex + 1; i < Candles.size(); i++)
        {
            // 손절 우선 계산
            if (Candles[i].Low <= signal.LosscutPrice)
            {
                result.EndCandleIndex = i;
                result.ExitPrice = signal.LosscutPrice;
                result.ExpectedProfit = result.ExitPrice - result.EnterPrice;
                break;
            }

            // 익절선 돌파 시, EMA1 < EMA2 될 때까지 포지션 유지
            if (takeProfit)
            {
                if (Indicators[i].EMA1 < Indicators[i].EMA2)
                {
                    result.EndCandleIndex = i;
                    result.ExitPrice = Candles[i].Close;
                    result.ExpectedProfit = result.ExitPrice - result.EnterPrice;
                    break;
                }
            }
            else if (Candles[i].High >= signal.TakeProfitPrice)
            {
                takeProfit = true;
            }
        }

        return result;
    };
};