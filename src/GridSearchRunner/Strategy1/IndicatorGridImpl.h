#pragma once
#include "Interface/IGrid.h"
#include "Strategy1Detail.h"
#include "Type.h"

#include <vector>

struct IndicatorGrid
{
    std::vector<Decimal<4>> kValue1Range;
    std::vector<Decimal<4>> kValue2Range;
    std::vector<Decimal<4>> kValue3Range;
    std::vector<Decimal<4>> kValue4Range;
};

class Strategy1IndicatorGridImpl : public IGrid<Strategy1IndicatorGridImpl, Strategy1IndicatorArg>
{
private:
    IndicatorGrid grid;

public:
    using Base = IGrid<Strategy1IndicatorGridImpl, Strategy1IndicatorArg>;
    using Base::ArgType;

    Strategy1IndicatorGridImpl(IndicatorGrid&& g) : grid(std::move(g)) {};

    u32 GetCombinationCountImpl() const noexcept
    {
        return grid.kValue1Range.size() * grid.kValue2Range.size() * grid.kValue3Range.size() * grid.kValue4Range.size();
    }

    std::vector<Strategy1IndicatorArg> GenerateCombinationsImpl() const noexcept
    {
        std::vector<Strategy1IndicatorArg> combinations;
        combinations.reserve(GetCombinationCountImpl());

        for (const auto& c1 : grid.kValue1Range)
        {
            for (const auto& c2 : grid.kValue2Range)
            {
                for (const auto& c3 : grid.kValue3Range)
                {
                    for (const auto& c4 : grid.kValue4Range)
                    {
                        combinations.push_back({c1, c2, c3, c4});
                    }
                }
            }
        }

        return combinations;
    }
};
