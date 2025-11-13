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
    strategy.SetIndicatorsImpl();
    end = Capture;
    PrintOut(std::format("{}", end - start));

    std::cout << strategy.GetCandleSize() << std::endl;
    std::cout << "Hello World!" << std::endl;
}