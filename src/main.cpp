#include "Analyze/GridSearchRunner.h"
#include "AopRunner.h"
#include "DataStructure/AtomicQueue.h"
#include "GridSearchHelper.h"
#include "Print.h"
#include "SignalGridImpl.h"
#include "Strategy1.h"
#include "Util/Atomic.h"
#include "args.h"

#include <algorithm>
#include <thread>

FORCE_NOINLINE void ProcessChunk(const std::vector<Strategy1SignalArg>& chunk, Strategy1& strategy, std::atomic<u32>& iterCount, OUT std::vector<Strategy1AnalyzeResult>& results, u32 workerID)
{
    auto id = workerID++;
    std::vector<Strategy1AnalyzeResult> analyzeResults;
    analyzeResults.reserve(chunk.size());
    for (const auto& item : chunk)
    {
        auto signals = strategy.MakeSignals(strategy.GetSignalStartIndex(), item);
        auto results = strategy.MakeResults(signals);
        auto before = iterCount.fetch_add(1);
        PrintOut(std::format("[{:2}] IterCount: {}", id, before + 1));

        Decimal<2> totalProfit = {0};
        for (auto& result : results)
        {
            totalProfit.Value += result.ExpectedProfit.Value;
        }
        analyzeResults.push_back({totalProfit, static_cast<u32>(results.size()), item});
    }

    size_t topN = MIN(RESULT_COUNT, analyzeResults.size());

    std::partial_sort(analyzeResults.begin(), analyzeResults.begin() + topN, analyzeResults.end(), [](const auto& a, const auto& b) { return a.TotalProfit.Value * b.SignalCount > b.TotalProfit.Value * a.SignalCount; });

    std::vector<Strategy1AnalyzeResult> res(analyzeResults.begin(), analyzeResults.begin() + topN);

    results = std::move(res);
}

int main()
{
    // Strategy, Grid, GridImpl, AnalyzeResult
    Strategy1 strategy;

    MeasureCycles([&strategy]() { strategy.SetCandlesImpl(Symbol::ETHUSDT, Interval::FiveMinutes); });
    MeasureCycles([&strategy]() { strategy.Ready(); });
    MeasureCycles([&strategy]() { strategy.SetIndicatorsImpl(targetIndicatorArgs); });

    SignalGrid signalGrid{.coeff1Range = GridSearch::MakeRange(Decimal<2>{5}, Decimal<2>{10}, Decimal<2>{1}),
                          .coeff2Range = GridSearch::MakeRange(Decimal<2>{5}, Decimal<2>{10}, Decimal<2>{1}),
                          .coeff3Range = GridSearch::MakeRange(Decimal<2>{5}, Decimal<2>{10}, Decimal<2>{1}),
                          .coeff4Range = GridSearch::MakeRange(Decimal<2>{5}, Decimal<2>{10}, Decimal<2>{1})};

    Strategy1SignalGridImpl gridImpl(std::move(signalGrid));

    auto runner = GridSearchRunner<Strategy1SignalGridImpl>(std::move(gridImpl));
    auto combChunks = runner.GenerateCombinations<CORE_COUNT>();

    std::atomic<u32> iterCount = 0;
    u32 workerID = 0;
    std::vector<std::thread> workerList;

    std::array<std::vector<Strategy1AnalyzeResult>, CORE_COUNT> results{};

    for (auto i = 0; i < combChunks.size(); i++)
    {
        workerList.push_back(std::thread(ProcessChunk, std::ref(combChunks[i]), std::ref(strategy), std::ref(iterCount), std::ref(results[i]), workerID++));
    }

    for (auto& t : workerList)
    {
        if (t.joinable())
            t.join();
    }

    std::vector<Strategy1AnalyzeResult> finalResult;
    for (auto& result : results)
    {
        for (auto& elem : result)
        {
            finalResult.push_back(elem);
        }
    }

    std::partial_sort(finalResult.begin(), finalResult.begin() + RESULT_COUNT, finalResult.end(), [](const auto& a, const auto& b) { return a.TotalProfit.Value * b.SignalCount > b.TotalProfit.Value * a.SignalCount; });

    finalResult.resize(RESULT_COUNT);

    std::cout << std::endl;
}