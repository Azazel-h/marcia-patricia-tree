#ifndef SILKWORM_BYTES_HPP
#define SILKWORM_BYTES_HPP

#ifdef __cplusplus

/*
   Copyright 2023 The Silkworm Authors

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

#include <cstdint>
#include <span>

#include "evmc/bytes.hpp"

namespace silkworm {

    using Bytes = evmc::bytes;

    class ByteView : public evmc::bytes_view {
    public:
        constexpr ByteView() noexcept = default;

        constexpr ByteView(const evmc::bytes_view& other) noexcept
                : evmc::bytes_view{other.data(), other.length()} {}

        ByteView(const Bytes& str) noexcept : evmc::bytes_view{str.data(), str.length()} {}

        constexpr ByteView(const uint8_t* data, size_type length) noexcept
                : evmc::bytes_view{data, length} {}

        template <std::size_t N>
        constexpr ByteView(const uint8_t (&array)[N]) noexcept : evmc::bytes_view{array, N} {}

        template <std::size_t N>
        constexpr ByteView(const std::array<uint8_t, N>& array) noexcept
                : evmc::bytes_view{array.data(), N} {}

        template <std::size_t Extent>
        constexpr ByteView(std::span<const uint8_t, Extent> span) noexcept
                : evmc::bytes_view{span.data(), span.size()} {}

        [[nodiscard]] bool is_null() const noexcept { return data() == nullptr; }
    };

    template <std::size_t Extent>
    using ByteSpan = std::span<uint8_t, Extent>;

}  // namespace silkworm

#endif // __cplusplus

// C interface
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

typedef struct {
    const uint8_t* data;
    size_t length;
} silkworm_ByteView;

typedef struct {
    uint8_t* data;
    size_t length;
    size_t capacity;
} silkworm_Bytes;

silkworm_ByteView silkworm_ByteView_create(const uint8_t* data, size_t length);
int silkworm_ByteView_is_null(const silkworm_ByteView* view);

silkworm_Bytes silkworm_Bytes_create(size_t initial_capacity);
void silkworm_Bytes_destroy(silkworm_Bytes* bytes);
int silkworm_Bytes_resize(silkworm_Bytes* bytes, size_t new_size);
int silkworm_Bytes_append(silkworm_Bytes* bytes, const uint8_t* data, size_t length);

typedef struct {
    uint8_t* data;
    size_t length;
} silkworm_ByteSpan;

silkworm_ByteSpan silkworm_ByteSpan_create(uint8_t* data, size_t length);

#ifdef __cplusplus
}
#endif

#endif // SILKWORM_BYTES_HPP_

/*
    В этой версии файла:

    Оригинальный C++ код остается внутри #ifdef __cplusplus.
    Добавлен C-интерфейс, который включает в себя:

    Определение структуры silkworm_ByteView, которая соответствует классу ByteView в C++.
    Функцию silkworm_ByteView_create для создания silkworm_ByteView.
    Функцию silkworm_ByteView_is_null для проверки, является ли silkworm_ByteView нулевым.
    Определение структуры silkworm_ByteSpan, которая соответствует ByteSpan в C++.
    Функцию silkworm_ByteSpan_create для создания silkworm_ByteSpan.


    C-интерфейс обернут в extern "C" блок, чтобы обеспечить C-совместимое связывание при компиляции C++ компилятором.
    Использованы макросы препроцессора для условной компиляции, чтобы этот заголовочный файл мог быть использован как в C, так и в C++ коде.
*/
/*
    UPD... В этой обновленной версии:

    Добавлен тип silkworm_Bytes, который представляет собой структуру с указателем на данные, длиной и емкостью. Это примерно соответствует функциональности std::basic_string.
    Добавлены функции для работы с silkworm_Bytes:

    silkworm_Bytes_create для создания нового объекта silkworm_Bytes.
    silkworm_Bytes_destroy для освобождения памяти, выделенной для silkworm_Bytes.
    silkworm_Bytes_resize для изменения размера silkworm_Bytes.
    silkworm_Bytes_append для добавления данных в конец silkworm_Bytes.

    Теперь silkworm_Bytes доступен для использования в других файлах, включая nibbles.hpp. Это позволяет корректно определить функции silkworm_pack_nibbles и silkworm_unpack_nibbles в nibbles.hpp, которые возвращают silkworm_Bytes.
 */