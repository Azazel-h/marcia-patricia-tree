#include "merkle-patricia-tree/common/bytes_to_string.hpp"
#include <cstring>

silkworm_Bytes silkworm_string_to_bytes(const char* str, size_t length) {
    silkworm_Bytes result = silkworm_Bytes_create(length);
    if (silkworm_Bytes_resize(&result, length)) {
        memcpy(result.data, str, length);
    }
    return result;
}

silkworm_ByteView silkworm_string_view_to_byte_view(const char* str, size_t length) {
    return silkworm_ByteView_create(reinterpret_cast<const uint8_t*>(str), length);
}

silkworm_ByteView silkworm_array_to_byte_view(const unsigned char* array, size_t size) {
    return silkworm_ByteView_create(array, size);
}

silkworm_StringView silkworm_byte_view_to_string_view(silkworm_ByteView view) {
    silkworm_StringView result;
    result.data = reinterpret_cast<const char*>(view.data);
    result.length = view.length;
    return result;
}