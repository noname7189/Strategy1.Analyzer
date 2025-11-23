# Strategy1.Analyzer

OHLCV 캔들 데이터 기반 매매 전략의 GridSearch 백테스팅 프레임워크

## 개요

Strategy1.Core를 기반으로 구축된 전략 분석 도구. 멀티스레드 GridSearch를 통한 파라미터 최적화와 백테스팅을 지원.

## 핵심 컴포넌트

### 전략 구현 (IAnalyzer)

Strategy1.Core의 `IAnalyzer` 인터페이스를 구현하여 전략을 정의.
```cpp
class Strategy1 : public IAnalyzer
    Strategy1, 
    Strategy1Indicator, 
    Strategy1Signal, 
    Strategy1Result, 
    CacheLineAllocator<Strategy1Indicator>
> {
    using IAnalyzerImpl = std::true_type;
    
    // 필수 구현
    u32 GetCandleSizeImpl() noexcept;
    void SetCandlesImpl(Symbol symbol, Interval interval) noexcept;
    Strategy1Indicator MakeIndicator(u32 index, const Strategy1IndicatorArg& arg) noexcept;
    Strategy1Signal MakeSignal(u32 index, const Strategy1SignalArg& arg) noexcept;
    Strategy1Result MakeResult(const Strategy1Signal& signal) noexcept;
};
```

### 데이터 구조체 정의

Base 구조체를 상속하여 전략별 타입 정의:
```cpp
// 지표
struct Strategy1Indicator : BaseIndicator {
    Decimal<4> EMA1;
    Decimal<4> EMA2;
    Decimal<4> EMA3;
    Decimal<4> EMA4;
};

// 신호
struct Strategy1Signal : BaseSignal<2> {
    // CandleIndex, Position, TakeProfitPrice, LosscutPrice 상속
};

// 결과
struct Strategy1Result : BaseResult<2> {
    // StartCandleIndex, EndCandleIndex, EnterPrice, ExitPrice, ExpectedProfit 상속
};

// 파라미터
struct Strategy1IndicatorArg {
    Decimal<4> KValue1, KValue2, KValue3, KValue4;
    u32 Period1, Period2, Period3, Period4;
};

struct Strategy1SignalArg {
    Decimal<2> coeff1, coeff2, coeff3, coeff4;
};

// 분석 결과
using Strategy1AnalyzeResult = AnalyzeResult<Strategy1SignalArg>;
```

### Grid 구현 (IGrid)

GridSearch를 위한 파라미터 조합 생성:
```cpp
class Strategy1SignalGridImpl : public IGrid<Strategy1SignalGridImpl, Strategy1SignalArg> {
    using GridImpl = std::true_type;
    using Base::ArgType;
    
    SignalGrid grid;
    
    u32 GetCombinationCountImpl() const noexcept;
    std::vector<Strategy1SignalArg> GenerateCombinationsImpl() const noexcept;
};

struct SignalGrid {
    std::vector<Decimal<2>> coeff1Range;
    std::vector<Decimal<2>> coeff2Range;
    std::vector<Decimal<2>> coeff3Range;
    std::vector<Decimal<2>> coeff4Range;
};
```

### GridAnalyzeRunner

멀티스레드 GridSearch 실행 엔진:
```cpp
template<StrategyType Strategy, typename IndicatorArg, typename SignalArg, 
         GridType GridImpl, ResultBase AnalyzeResult>
class GridAnalyzeRunner {
    void SetCandles(Symbol symbol, Interval interval) noexcept;
    void SetIndicators(IndicatorArg arg) noexcept;
    void Run() noexcept;
};
```

**동작 방식:**
1. 파라미터 조합을 CORE_COUNT 개의 청크로 분할
2. 각 청크를 별도 스레드에서 병렬 처리
3. 각 스레드는 top N 결과 수집
4. 전체 결과를 병합하여 최종 top N 선정

## 사용 예시

### 기본 사용법
```cpp
#include "AnalyzeRunner.h"
#include "SignalGridImpl.h"
#include "Strategy1.h"

int main() {
    using AnalyzeRunner = GridAnalyzeRunner
        Strategy1, 
        Strategy1IndicatorArg, 
        Strategy1SignalArg, 
        Strategy1SignalGridImpl, 
        Strategy1AnalyzeResult
    >;
    
    // Grid 범위 설정
    SignalGrid signalGrid{
        .coeff1Range = GridSearch::MakeRange(Decimal<2>{5}, Decimal<2>{10}, Decimal<2>{1}),
        .coeff2Range = GridSearch::MakeRange(Decimal<2>{5}, Decimal<2>{10}, Decimal<2>{1}),
        .coeff3Range = GridSearch::MakeRange(Decimal<2>{5}, Decimal<2>{10}, Decimal<2>{1}),
        .coeff4Range = GridSearch::MakeRange(Decimal<2>{5}, Decimal<2>{10}, Decimal<2>{1})
    };
    
    Strategy1SignalGridImpl gridImpl(std::move(signalGrid));
    
    // Runner 실행
    AnalyzeRunner runner(std::move(gridImpl));
    runner.SetCandles(Symbol::ETHUSDT, Interval::FiveMinutes);
    runner.SetIndicators(targetIndicatorArgs);
    runner.Run();
}
```

### 전략 구현 단계

1. **데이터 구조체 정의** - Base 구조체 상속
2. **IAnalyzer 구현** - 5개 메서드 구현
3. **IGrid 구현** - 파라미터 조합 생성
4. **GridAnalyzeRunner 실행** - 멀티스레드 최적화

