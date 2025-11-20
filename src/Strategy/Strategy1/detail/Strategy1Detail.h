#pragma once
#include "Analyze/AnalyzeResult.h"
#include "BaseStruct/BaseIndicator.h"
#include "BaseStruct/BaseResult.h"
#include "BaseStruct/BaseSignal.h"

struct Strategy1IndicatorArg
{
    Decimal<4> KValue1;
    Decimal<4> KValue2;
    Decimal<4> KValue3;
    Decimal<4> KValue4;
    u32 Period1;
    u32 Period2;
    u32 Period3;
    u32 Period4;
};

struct Strategy1SignalArg
{
    Decimal<2> coeff1;
    Decimal<2> coeff2;
    Decimal<2> coeff3;
    Decimal<2> coeff4;
};

struct Strategy1Indicator : BaseIndicator
{
    Decimal<4> EMA1;
    Decimal<4> EMA2;
    Decimal<4> EMA3;
    Decimal<4> EMA4;
};

struct Strategy1Signal : BaseSignal<2>
{
};

struct Strategy1Result : BaseResult<2>
{
};

using Strategy1AnalyzeResult = AnalyzeResult<Strategy1SignalArg>;