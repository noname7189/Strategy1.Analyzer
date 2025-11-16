#pragma once

#include "Macro.h"
#include "Strategy1Detail.h"

template <u32 CandleScale, u32 IndicatorScale>
constexpr Decimal<IndicatorScale> CalculateEMA(Decimal<IndicatorScale> kDecimal, Decimal<CandleScale> currentPrice, Decimal<IndicatorScale> previousIndic)
{
    const i64 k = kDecimal.Value;
    const i64 divisor = kDecimal.Divisor;
    const i64 one_minus_k = divisor - k;

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

template <u32 IndicatorScale>
constexpr Decimal<IndicatorScale> CalculateEMA(Decimal<IndicatorScale> kDecimal, Decimal<2> currentPrice, Decimal<IndicatorScale> previousIndic)
{
    return CalculateEMA<2, IndicatorScale>(kDecimal, currentPrice, previousIndic);
}
