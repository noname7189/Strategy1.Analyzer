#pragma once

#include "Type.h"

#include <vector>

namespace GridSearch {
template <u32 Scale>
std::vector<Decimal<Scale>> MakeRange(Decimal<Scale> start, Decimal<Scale> end, Decimal<Scale> step)
{
    std::vector<Decimal<Scale>> result;

    i64 count = (end.Value - start.Value) / step.Value + 1;
    if (count > 0)
    {
        result.reserve(static_cast<size_t>(count));
    }

    for (i64 v = start.Value; v <= end.Value; v += step.Value)
    {
        result.push_back(Decimal<Scale>{v});
    }

    return result;
}
} // namespace GridSearch
