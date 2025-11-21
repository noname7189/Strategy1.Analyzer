#pragma once

#include "Analyze/GridSearchRunner.h"
#include "Concept/StrategyConcept.h"
#include "Constant.h"
#include "InlineFunc.h"
#include "Macro.h"
#include "Print.h"
#include "Type.h"
#include "Util/Atomic.h"

#include <algorithm>
#include <thread>

template <StrategyType Strategy, typename IndicatorArg, typename SignalArg, GridType GridImpl, ResultBase AnalyzeResult>
class GridAnalyzeRunner
{
    Strategy strategy;
    GridSearchRunner<GridImpl> runner;

public:
    GridAnalyzeRunner(GridImpl&& impl) : strategy{}, runner(std::move(impl)) {};

    void SetCandles(Symbol symbol, Interval interval) noexcept
    {
        strategy.SetCandlesImpl(symbol, interval);
    }

    void SetIndicators(IndicatorArg arg) noexcept
    {
        strategy.Ready();
        strategy.SetIndicatorsImpl(arg);
    }

    void ProcessChunk(const std::vector<SignalArg>& chunk, Strategy& strategy, std::atomic<u32>& iterCount, OUT std::vector<AnalyzeResult>& results, u32 workerID) noexcept
    {
        auto id = workerID++;
        std::vector<AnalyzeResult> analyzeResults;
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

        std::vector<AnalyzeResult> res(analyzeResults.begin(), analyzeResults.begin() + topN);

        results = std::move(res);
    }

    void Run() noexcept
    {
        auto combChunks = runner.template GenerateCombinations<CORE_COUNT>();
        std::atomic<u32> iterCount = 0;
        u32 workerID = 0;
        std::vector<std::thread> workerList;

        std::array<std::vector<AnalyzeResult>, CORE_COUNT> results{};

        for (auto i = 0; i < combChunks.size(); i++)
        {
            workerList.push_back(std::thread(&GridAnalyzeRunner::ProcessChunk, this, std::ref(combChunks[i]), std::ref(strategy), std::ref(iterCount), std::ref(results[i]), workerID++));
        }

        for (auto& t : workerList)
        {
            if (t.joinable())
                t.join();
        }

        std::vector<AnalyzeResult> finalResult;
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
};