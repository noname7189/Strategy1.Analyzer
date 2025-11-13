#pragma once
#include "BaseStruct/BaseIndicator.h"
#include "BaseStruct/BaseResult.h"
#include "BaseStruct/BaseSignal.h"

#include <compare>

struct Strategy1Arg
{
    Decimal<2> coeff1;
    Decimal<2> coeff2;
    Decimal<2> coeff3;
    Decimal<2> coeff4;
};

struct Strategy1Indicator : BaseIndicator
{
    Decimal<4> EMA5;
    Decimal<4> EMA20;
    Decimal<4> EMA60;
    Decimal<4> EMA120;
};

struct Strategy1Signal : BaseSignal<2>
{
};

struct Strategy1Result : BaseResult<2>
{
};
