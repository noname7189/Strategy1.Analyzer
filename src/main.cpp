#include "AopRunner.h"
#include "DataStructure/AtomicQueue.h"
#include "Print.h"
#include "Strategy1.h"
#include "args.h"

int main()
{
    Strategy1 strategy;

    MeasureCycles([&strategy]() { strategy.SetCandlesImpl(Symbol::ETHUSDT, Interval::FiveMinutes); });
    MeasureCycles([&strategy]() { strategy.Ready(); });
    MeasureCycles([&strategy]() { strategy.SetIndicatorsImpl(targetIndicatorArgs); });
    auto signals = MeasureCycles([&strategy]() { return strategy.MakeSignals(strategy.GetSignalStartIndex(), targetSignalArgs); });

    auto results = MeasureCycles([&strategy, &signals]() { return strategy.MakeResults(signals); });

    std::cout << "Hello World!" << std::endl;
}