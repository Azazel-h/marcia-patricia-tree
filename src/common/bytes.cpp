#include "merkle-patricia-tree/common/bytes.hpp"
#include <cstdlib>
#include <cstring>
#include <new>

silkworm_ByteView silkworm_ByteView_create(const uint8_t* data, size_t length) {
    silkworm_ByteView view;
    view.data = data;
    view.length = length;
    return view;
}

int silkworm_ByteView_is_null(const silkworm_ByteView* view) {
    if (view == nullptr) {
        return 1;  // true, view is null
    }
    silkworm::ByteView cpp_view(view->data, view->length);
    return cpp_view.is_null() ? 1 : 0;
}

silkworm_Bytes silkworm_Bytes_create(size_t initial_capacity) {
    silkworm_Bytes bytes;
    bytes.data = static_cast<uint8_t*>(std::malloc(initial_capacity));
    if (bytes.data == nullptr) {
        bytes.length = 0;
        bytes.capacity = 0;
    } else {
        bytes.length = 0;
        bytes.capacity = initial_capacity;
    }
    return bytes;
}

void silkworm_Bytes_destroy(silkworm_Bytes* bytes) {
    if (bytes && bytes->data) {
        std::free(bytes->data);
        bytes->data = nullptr;
        bytes->length = 0;
        bytes->capacity = 0;
    }
}

int silkworm_Bytes_resize(silkworm_Bytes* bytes, size_t new_size) {
    if (!bytes) return 0;

    if (new_size > bytes->capacity) {
        size_t new_capacity = new_size * 2;  // Double the capacity
        uint8_t* new_data = static_cast<uint8_t*>(std::realloc(bytes->data, new_capacity));
        if (new_data == nullptr) return 0;  // Reallocation failed

        bytes->data = new_data;
        bytes->capacity = new_capacity;
    }

    bytes->length = new_size;
    return 1;  // Success
}

int silkworm_Bytes_append(silkworm_Bytes* bytes, const uint8_t* data, size_t length) {
    if (!bytes || !data) return 0;

    size_t new_length = bytes->length + length;
    if (silkworm_Bytes_resize(bytes, new_length) == 0) return 0;  // Resize failed

    std::memcpy(bytes->data + bytes->length - length, data, length);
    return 1;  // Success
}

silkworm_ByteSpan silkworm_ByteSpan_create(uint8_t* data, size_t length) {
    silkworm_ByteSpan span;
    span.data = data;
    span.length = length;
    return span;
}

/*
    Эта реализация предоставляет следующую функциональность:

    silkworm_ByteView_create и silkworm_ByteView_is_null реализованы как раньше.
    silkworm_Bytes_create создает новый объект silkworm_Bytes с заданной начальной емкостью, используя malloc для выделения памяти.
    silkworm_Bytes_destroy освобождает память, выделенную для silkworm_Bytes, используя free.
    silkworm_Bytes_resize изменяет размер silkworm_Bytes. Если новый размер больше текущей емкости, функция выделяет новую память с удвоенной емкостью.
    silkworm_Bytes_append добавляет данные в конец silkworm_Bytes, при необходимости увеличивая емкость.
    silkworm_ByteSpan_create реализована как раньше.
 */