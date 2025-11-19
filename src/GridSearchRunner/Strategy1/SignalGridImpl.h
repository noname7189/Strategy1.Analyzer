#pragma once
#include "Interface/IGrid.h"
#include "Strategy1Detail.h"

#include <vector>

struct SignalGrid
{
    std::vector<Decimal<2>> coeff1Range;
    std::vector<Decimal<2>> coeff2Range;
    std::vector<Decimal<2>> coeff3Range;
    std::vector<Decimal<2>> coeff4Range;
};

class Strategy1SignalGridImpl : public IGrid<Strategy1SignalGridImpl, Strategy1SignalArg>
{
private:
    SignalGrid grid;

public:
    using Base = IGrid<Strategy1SignalGridImpl, Strategy1SignalArg>;
    using Base::ArgType;

    Strategy1SignalGridImpl(SignalGrid&& g) : grid(std::move(g)) {};

    u32 GetCombinationCountImpl() const noexcept
    {
        return grid.coeff1Range.size() * grid.coeff2Range.size() * grid.coeff3Range.size() * grid.coeff4Range.size();
    }

    std::vector<Strategy1SignalArg> GenerateCombinationsImpl() const noexcept
    {
        std::vector<Strategy1SignalArg> combinations;
        combinations.reserve(GetCombinationCountImpl());

        for (const auto& c1 : grid.coeff1Range)
        {
            for (const auto& c2 : grid.coeff2Range)
            {
                for (const auto& c3 : grid.coeff3Range)
                {
                    for (const auto& c4 : grid.coeff4Range)
                    {
                        combinations.push_back({c1, c2, c3, c4});
                    }
                }
            }
        }

        return combinations;
    }
};
