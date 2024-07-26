#ifndef SILKWORM_BASE_HPP
#define SILKWORM_BASE_HPP

#ifdef __cplusplus
/*
   Copyright 2022 The Silkworm Authors

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#pragma once

// The most common and basic macros, concepts, types, and constants.

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <tuple>

#include "intx/intx.hpp"

// TODO(yperbasis): get rid of this hack
#if defined(_MSC_VER)
#define SILKWORM_CONSTINIT
#else
#define SILKWORM_CONSTINIT constinit
#endif

#if defined(__wasm__)
#define SILKWORM_THREAD_LOCAL static
#else
#define SILKWORM_THREAD_LOCAL thread_local
#endif

namespace silkworm {

using namespace std::string_view_literals;

template <class T>
concept UnsignedIntegral = std::unsigned_integral<T> || std::same_as<T, intx::uint128> ||
                           std::same_as<T, intx::uint256> || std::same_as<T, intx::uint512>;

using BlockNum = uint64_t;
using BlockNumRange = std::pair<BlockNum, BlockNum>;
using BlockTime = uint64_t;

inline constexpr BlockNum kEarliestBlockNumber{0ul};

inline constexpr size_t kAddressLength{20};

inline constexpr size_t kHashLength{32};

// https://en.wikipedia.org/wiki/Binary_prefix
inline constexpr uint64_t kKibi{1024};
inline constexpr uint64_t kMebi{1024 * kKibi};
inline constexpr uint64_t kGibi{1024 * kMebi};
inline constexpr uint64_t kTebi{1024 * kGibi};

inline constexpr uint64_t kGiga{1'000'000'000};   // = 10^9
inline constexpr uint64_t kEther{kGiga * kGiga};  // = 10^18

constexpr uint64_t operator"" _Kibi(unsigned long long x) { return x * kKibi; }
constexpr uint64_t operator"" _Mebi(unsigned long long x) { return x * kMebi; }
constexpr uint64_t operator"" _Gibi(unsigned long long x) { return x * kGibi; }
constexpr uint64_t operator"" _Tebi(unsigned long long x) { return x * kTebi; }

}  // namespace silkworm

#endif // __cplusplus

// C interface
#ifdef __cplusplus
extern "C" {
#endif

typedef uint64_t BlockNum;
typedef uint64_t BlockTime;

typedef struct {
    BlockNum first;
    BlockNum second;
} BlockNumRange;

#define SILKWORM_EARLIEST_BLOCK_NUMBER 0ULL
#define SILKWORM_ADDRESS_LENGTH 20
#define SILKWORM_HASH_LENGTH 32

#define SILKWORM_KIBI 1024ULL
#define SILKWORM_MEBI (1024ULL * SILKWORM_KIBI)
#define SILKWORM_GIBI (1024ULL * SILKWORM_MEBI)
#define SILKWORM_TEBI (1024ULL * SILKWORM_GIBI)

#define SILKWORM_GIGA 1000000000ULL
#define SILKWORM_ETHER (SILKWORM_GIGA * SILKWORM_GIGA)

// Function to convert units
uint64_t silkworm_to_kibi(uint64_t x);
uint64_t silkworm_to_mebi(uint64_t x);
uint64_t silkworm_to_gibi(uint64_t x);
uint64_t silkworm_to_tebi(uint64_t x);

#ifdef __cplusplus
}
#endif

#endif // SILKWORM_BASE_HPP

/*
    В этой версии файла:

    Оригинальный C++ код остается внутри #ifdef __cplusplus.
    Добавлен C-интерфейс, который включает в себя:

    Определения типов BlockNum, BlockTime и BlockNumRange.
    Константы, эквивалентные C++ константам, определенные как макросы.
    Функции для конвертации единиц, эквивалентные операторам C++.

    C-интерфейс обернут в extern "C" блок, чтобы обеспечить C-совместимое связывание при компиляции C++ компилятором.
    Использованы макросы препроцессора для условной компиляции, чтобы этот заголовочный файл мог быть использован как в C, так и в C++ коде.
 */