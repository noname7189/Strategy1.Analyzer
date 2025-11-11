#pragma once
#include "BaseStruct/BaseIndicator.h"
#include "BaseStruct/BaseResult.h"
#include "BaseStruct/BaseSignal.h"

struct Strategy1Indicator : BaseIndicator
{
    DecimalNull Indicator1;
    DecimalNull Indicator2;
    DecimalNull Indicator3;
    DecimalNull Indicator4;
    DecimalNull Indicator5;
    DecimalNull Indicator6;
    DecimalNull Indicator7;
};

struct Strategy1Signal : BaseSignal
{
};

struct Strategy1SignalArg : BaseSignal
{
    Decimal arg1;
    Decimal arg2;
    Decimal arg3;
    Decimal arg4;
};

struct Strategy1Result : BaseResult
{
};
