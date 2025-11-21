#include "AnalyzeRunner.h"
#include "GridSearchHelper.h"
#include "SignalGridImpl.h"
#include "Strategy1.h"
#include "args.h"

int main()
{
    using AnalyzeRunner = GridAnalyzeRunner<Strategy1, Strategy1IndicatorArg, Strategy1SignalArg, Strategy1SignalGridImpl, Strategy1AnalyzeResult>;

    SignalGrid signalGrid{.coeff1Range = GridSearch::MakeRange(Decimal<2>{5}, Decimal<2>{10}, Decimal<2>{1}),
                          .coeff2Range = GridSearch::MakeRange(Decimal<2>{5}, Decimal<2>{10}, Decimal<2>{1}),
                          .coeff3Range = GridSearch::MakeRange(Decimal<2>{5}, Decimal<2>{10}, Decimal<2>{1}),
                          .coeff4Range = GridSearch::MakeRange(Decimal<2>{5}, Decimal<2>{10}, Decimal<2>{1})};

    Strategy1SignalGridImpl gridImpl(std::move(signalGrid));

    AnalyzeRunner runner(std::move(gridImpl));
    runner.SetCandles(Symbol::ETHUSDT, Interval::FiveMinutes);
    runner.SetIndicators(targetIndicatorArgs);
    runner.Run();
}