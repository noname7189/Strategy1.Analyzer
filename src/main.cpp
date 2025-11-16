#include "DataStructure/AtomicQueue.h"
#include "Print.h"
#include "Strategy1.h"

int main()
{
    Strategy1 strategy;
    auto start = Capture;
    strategy.SetCandlesImpl(Symbol::ETHUSDT, Interval::FiveMinutes);
    auto end = Capture;

    PrintOut(std::format("{}", end - start));

    start = Capture;

    strategy.Ready();

    {
        Strategy1IndicatorArg indicatorArgs;

        constexpr Decimal<4> k5{3333};
        constexpr Decimal<4> k20{952};
        constexpr Decimal<4> k60{333};
        constexpr Decimal<4> k120{165};

        indicatorArgs.KValue1 = k5;
        indicatorArgs.Period1 = 5;

        indicatorArgs.KValue2 = k20;
        indicatorArgs.Period2 = 20;

        indicatorArgs.KValue3 = k60;
        indicatorArgs.Period3 = 60;

        indicatorArgs.KValue4 = k120;
        indicatorArgs.Period4 = 120;
        strategy.SetIndicatorsImpl(indicatorArgs);
    }

    end = Capture;
    PrintOut(std::format("{}", end - start));

    std::cout << strategy.GetCandleSize() << std::endl;
    std::cout << "Hello World!" << std::endl;
}