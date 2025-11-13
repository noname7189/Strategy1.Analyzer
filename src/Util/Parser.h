#pragma once

#include "Macro.h"
#include "Print.h"
#include "Type.h"

class Parser
{
private:
    static FORCE_INLINE u64 Parse8Digits(const char* str) noexcept
    {
        u64 val;
        FAST_MEMCPY(&val, str, 8);

        // ASCII '0'을 빼기
        val -= 0x3030303030303030ULL;

        // 각 자리 결합
        val = (val * 10 + (val >> 8)) & 0x00FF00FF00FF00FFULL;
        val = (val * 100 + (val >> 16)) & 0x0000FFFF0000FFFFULL;
        val = (val * 10000 + (val >> 32)) & 0x00000000FFFFFFFFULL;

        return val;
    }

public:
    static FORCE_INLINE std::string RemoveControlChars(const std::string& input)
    {
        std::string result;
        for (char c : input)
        {
            if (std::isprint(c) || std::isspace(c))
            {
                result += c;
            }
        }
        return result;
    }

    static FORCE_INLINE u64 ToU64(const char* str, int n) noexcept
    {
        u64 result = 0;

        while (n >= 8)
        {
            result = result * 100000000ULL + Parse8Digits(str);
            str += 8;
            n -= 8;
        }

        while (n > 0)
        {
            result = result * 10 + (*str - '0');
            ++str;
            --n;
        }

        return result;
    }

    static FORCE_INLINE DateTime ToDateTime(const char* str, int n) noexcept
    {
        return static_cast<DateTime>(ToU64(str, n));
    }

    template <int DecimalPlaces>
    static FORCE_INLINE Decimal<DecimalPlaces> ToDecimal(const char* str, int len) noexcept
    {
        bool negative = (len > 0 && str[0] == '-');
        int i = negative ? 1 : 0;

        int dotPos = i;
        while (dotPos < len && str[dotPos] != '.')
            ++dotPos;

        if (unlikely(dotPos == i))
        {
            PrintErr(std::format("Wrong Decimal<{}> dotPos format: {}", DecimalPlaces, std::string(str, len)));
            exit(1);
        }

        // 정수 부분
        i64 intPart = 0;
        for (int j = i; j < dotPos; ++j)
        {
            char c = str[j];
            if (unlikely(c < '0' || c > '9'))
            {
                PrintErr(std::format("Wrong Decimal<{}> interger format: {}", DecimalPlaces, std::string(str, len)));
                exit(1);
            }
            intPart = intPart * 10 + (c - '0');
        }

        if (unlikely(dotPos >= len))
        {
            Decimal<DecimalPlaces> result{intPart * Decimal<DecimalPlaces>::Divisor};
            return negative ? Decimal<DecimalPlaces>{-result.Value} : result;
        }

        if (unlikely(len - dotPos - 1 != DecimalPlaces))
        {
            PrintErr(std::format("Wrong Decimal<{}> dotPos format: {}", DecimalPlaces, std::string(str, len)));
            exit(1);
        }

        // 소수 부분
        i64 decimalPart = 0;
        i = dotPos + 1;
        for (int d = 0; d < DecimalPlaces; ++d)
        {
            char c = str[i++];
            if (unlikely(c < '0' || c > '9'))
            {
                PrintErr(std::format("Wrong Decimal<{}> decimal format: {}", DecimalPlaces, std::string(str, len)));
                exit(1);
            }
            decimalPart = decimalPart * 10 + (c - '0');
        }

        i64 value = intPart * Decimal<DecimalPlaces>::Divisor + decimalPart;
        return Decimal<DecimalPlaces>{negative ? -value : value};
    }
};
