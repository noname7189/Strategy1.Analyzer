#include "Analyze/GridSearchRunner.h"
#include "AopRunner.h"
#include "DataStructure/AtomicQueue.h"
#include "GridSearchHelper.h"
#include "Print.h"
#include "SignalGridImpl.h"
#include "Strategy1.h"
#include "Util/Atomic.h"
#include "args.h"

#include <thread>

FORCE_NOINLINE void ProcessChunk(const std::vector<Strategy1SignalArg>& chunk, Strategy1& strategy, std::atomic<u32>& iterCount, u32 workerID)
{
    auto id = workerID++;
    for (const auto& item : chunk)
    {
        auto signals = strategy.MakeSignals(strategy.GetSignalStartIndex(), item);
        auto results = strategy.MakeResults(signals);
        auto before = iterCount.fetch_add(1);

        PrintOut(std::format("[{:2}] IterCount: {}", id, before + 1));
    }
}

int main()
{
    Strategy1 strategy;

    MeasureCycles([&strategy]() { strategy.SetCandlesImpl(Symbol::ETHUSDT, Interval::FiveMinutes); });
    MeasureCycles([&strategy]() { strategy.Ready(); });
    MeasureCycles([&strategy]() { strategy.SetIndicatorsImpl(targetIndicatorArgs); });

    SignalGrid signalGrid{.coeff1Range = GridSearch::MakeRange(Decimal<2>{5}, Decimal<2>{20}, Decimal<2>{1}),
                          .coeff2Range = GridSearch::MakeRange(Decimal<2>{5}, Decimal<2>{20}, Decimal<2>{1}),
                          .coeff3Range = GridSearch::MakeRange(Decimal<2>{5}, Decimal<2>{20}, Decimal<2>{1}),
                          .coeff4Range = GridSearch::MakeRange(Decimal<2>{5}, Decimal<2>{20}, Decimal<2>{1})};

    Strategy1SignalGridImpl gridImpl(std::move(signalGrid));

    auto runner = GridSearchRunner<Strategy1SignalGridImpl>(std::move(gridImpl));
    auto combChunks = runner.GenerateCombinations<CORE_COUNT>();

    std::atomic<u32> iterCount = 0;
    u32 workerID = 0;
    std::vector<std::thread> workerList;

    for (const auto& chunk : combChunks)
    {
        workerList.push_back(std::thread(ProcessChunk, std::ref(chunk), std::ref(strategy), std::ref(iterCount), workerID++));
    }

    for (auto& t : workerList)
    {
        if (t.joinable())
            t.join();
    }
}