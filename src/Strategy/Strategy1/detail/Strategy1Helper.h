#pragma once

#include "Macro.h"
#include "Strategy1Detail.h"

template <u32 CandleScale, u32 IndicatorScale, auto K>
constexpr Decimal<IndicatorScale> CalculateEMA(Decimal<CandleScale> currentPrice, Decimal<IndicatorScale> previousIndic)
{
    constexpr i64 k = K.Value;
    constexpr i64 divisor = K.Divisor;
    constexpr i64 one_minus_k = divisor - k;

    if (unlikely(previousIndic.Value == Decimal<IndicatorScale>::Null))
    {
        return currentPrice.template rescale<IndicatorScale>();
    }

    if constexpr (CandleScale == IndicatorScale)
    {
        return {(currentPrice.Value * k + previousIndic.Value * one_minus_k) / divisor};
    }
    else if constexpr (CandleScale > IndicatorScale)
    {
        i64 scaledCurrent = currentPrice.Value / Pow10<CandleScale - IndicatorScale>::value;
        return {(scaledCurrent * k + previousIndic.Value * one_minus_k) / divisor};
    }
    else
    {
        i64 scaledCurrent = currentPrice.Value * Pow10<IndicatorScale - CandleScale>::value;
        return {(scaledCurrent * k + previousIndic.Value * one_minus_k) / divisor};
    }
}

template <u32 IndicatorScale, auto K>
constexpr Decimal<IndicatorScale> CalculateEMA(Decimal<2> currentPrice, Decimal<IndicatorScale> previousIndic)
{
    return CalculateEMA<2, IndicatorScale, K>(currentPrice, previousIndic);
}
