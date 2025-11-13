#pragma once
#include "BaseStruct/BaseCandle.h"
#include "Interface/IAnalyzer.h"
#include "Strategy1Detail.h"
#include "Strategy1Helper.h"

class Strategy1 : public IAnalyzer<Strategy1, Strategy1Indicator, Strategy1Signal, Strategy1Result>
{
    std::vector<BaseCandle<2>> Candles{};
    std::vector<Strategy1Indicator> Indicators{};

public:
    void SetCandlesImpl(Symbol symbol, Interval interval) noexcept;

    void SetIndicatorsImpl() noexcept;

    FORCE_INLINE u32 GetCandleSizeImpl() noexcept
    {
        return Candles.size();
    }

    FORCE_INLINE Strategy1Indicator SetIndicator(u32 index) noexcept
    {
        Strategy1Indicator indicator;
        indicator.CandleIndex = index;

        constexpr Decimal<4> k5{3333};
        constexpr Decimal<4> k20{952};
        constexpr Decimal<4> k60{333};
        constexpr Decimal<4> k120{165};

        if (unlikely(index < 119))
        {
            constexpr auto null = Decimal<4>::null();
            indicator.EMA5 = (index >= 4) ? CalculateEMA<4, k5>(Candles[index].Close, Indicators[index - 1].EMA5) : null;
            indicator.EMA20 = (index >= 19) ? CalculateEMA<4, k20>(Candles[index].Close, Indicators[index - 1].EMA20) : null;
            indicator.EMA60 = (index >= 59) ? CalculateEMA<4, k60>(Candles[index].Close, Indicators[index - 1].EMA60) : null;
            indicator.EMA120 = null;
        }
        else
        {
            indicator.EMA5 = CalculateEMA<4, k5>(Candles[index].Close, Indicators[index - 1].EMA5);
            indicator.EMA20 = CalculateEMA<4, k20>(Candles[index].Close, Indicators[index - 1].EMA20);
            indicator.EMA60 = CalculateEMA<4, k60>(Candles[index].Close, Indicators[index - 1].EMA60);
            indicator.EMA120 = CalculateEMA<4, k120>(Candles[index].Close, Indicators[index - 1].EMA120);
        }

        return indicator;
    };

    FORCE_INLINE Strategy1Signal MakeSignal(u32 index, const Strategy1Arg& arg) noexcept
    {
        Strategy1Signal signal;
        signal.CandleIndex = NAV<u32>();

        // TODO: 구현

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

            // 익절선 돌파 시, EMA5 < EMA20 될 때까지 포지션 유지
            if (takeProfit)
            {
                if (Indicators[i].EMA5 < Indicators[i].EMA20)
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