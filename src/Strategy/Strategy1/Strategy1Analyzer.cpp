#include "Strategy1Analyzer.h"

#include "Constant.h"
#include "Parser.h"
#include "Print.h"

#include <format>
#include <fstream>
#include <sstream>

using Indicator = Strategy1::Strategy1Indicator;
using Signal = Strategy1::Strategy1Signal;
using SignalArg = Strategy1::Strategy1SignalArg;
using Result = Strategy1::Strategy1Result;

void Strategy1Analyzer::SetCandlesImpl(Symbol symbol, Interval interval) noexcept
{
    std::string fileName = std::format("../src/Data/{}_{}.csv", SymbolStr[static_cast<int>(symbol)], IntervalStr[static_cast<int>(interval)]);
    std::ifstream file(fileName);

    if (!file.is_open())
    {
        PrintErr("Open file failed");
        exit(1);
    }

    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> elements;

        while (std::getline(ss, item, ','))
        {
            elements.push_back(item);
        }

        std::string timeStr = Parser::RemoveControlChars(elements[0]);
        std::string openStr = Parser::RemoveControlChars(elements[1]);
        std::string highStr = Parser::RemoveControlChars(elements[2]);
        std::string lowStr = Parser::RemoveControlChars(elements[3]);
        std::string closeStr = Parser::RemoveControlChars(elements[4]);
        std::string volStr = Parser::RemoveControlChars(elements[5]);

        auto time = Parser::ToDateTime(timeStr.c_str(), timeStr.size());
        auto open = Parser::ToDecimal<2>(openStr.c_str(), openStr.size());
        auto high = Parser::ToDecimal<2>(highStr.c_str(), highStr.size());
        auto low = Parser::ToDecimal<2>(lowStr.c_str(), lowStr.size());
        auto close = Parser::ToDecimal<2>(closeStr.c_str(), closeStr.size());
        auto volume = Parser::ToU64(volStr.c_str(), volStr.size());

        Candles.push_back({.Time = time, .Open = open, .High = high, .Low = low, .Close = close, .Volume = volume});
    }
}

std::vector<Indicator> Strategy1Analyzer::MakeIndicatorsImpl() noexcept
{
    std::vector<Indicator> indicators;
    indicators.reserve(Candles.size());

    for (int i = 0; i < Candles.size(); i++)
    {
        Indicator indic;
        indic.CandleIndex = i;
        // TODO: 구현
        indicators.push_back(indic);
    }

    return indicators;
}

std::vector<Signal> Strategy1Analyzer::MakeSignalsImpl(SignalArg&& arg) noexcept
{

    std::vector<Signal> signals;
    int estimatedCount = Candles.size() / 50;
    signals.reserve(estimatedCount);
    // TODO: 구현

    return signals;
}

std::vector<Result> Strategy1Analyzer::MakeResultsImpl() noexcept
{
    std::vector<Result> results;
    // TODO: 구현
    return results;
}
