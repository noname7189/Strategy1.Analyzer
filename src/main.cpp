#include "DataStructure/AtomicQueue.h"
#include "Strategy1.h"

#include <iostream>

int main()
{
    Strategy1 strategy;
    strategy.SetCandlesImpl(Symbol::ETHUSDT, Interval::FiveMinutes);
    std::cout << "Hello World!" << std::endl;
}