#pragma once
#include "Strategy1Detail.h"

constexpr Decimal<4> k5{3333};
constexpr Decimal<4> k20{952};
constexpr Decimal<4> k60{333};
constexpr Decimal<4> k120{165};

constexpr Strategy1IndicatorArg targetIndicatorArgs{k5, k20, k60, k120, 5, 20, 60, 120};

constexpr Strategy1SignalArg targetSignalArgs{Decimal<2>{10}, Decimal<2>{10}, Decimal<2>{10}, Decimal<2>{10}};
