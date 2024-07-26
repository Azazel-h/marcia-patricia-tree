#include "merkle-patricia-tree/common/endian.hpp"
#include "merkle-patricia-tree/common/util.hpp"

namespace silkworm::endian {

ByteView to_big_compact(const uint64_t value) {
    if (!value) {
        return {};
    }
    SILKWORM_THREAD_LOCAL uint8_t full_be[sizeof(uint64_t)];
    store_big_u64(&full_be[0], value);
    return zeroless_view(full_be);
}

ByteView to_big_compact(const intx::uint256& value) {
    if (!value) {
        return {};
    }
    SILKWORM_THREAD_LOCAL uint8_t full_be[sizeof(intx::uint256)];
    intx::be::store(full_be, value);
    return zeroless_view(full_be);
}

}  // namespace silkworm::endian

uint16_t silkworm_endian_load_big_u16(const uint8_t* src) {
    return silkworm::endian::load_big_u16(src);
}

uint32_t silkworm_endian_load_big_u32(const uint8_t* src) {
    return silkworm::endian::load_big_u32(src);
}

uint64_t silkworm_endian_load_big_u64(const uint8_t* src) {
    return silkworm::endian::load_big_u64(src);
}

uint16_t silkworm_endian_load_little_u16(const uint8_t* src) {
    return silkworm::endian::load_little_u16(src);
}

uint32_t silkworm_endian_load_little_u32(const uint8_t* src) {
    return silkworm::endian::load_little_u32(src);
}

uint64_t silkworm_endian_load_little_u64(const uint8_t* src) {
    return silkworm::endian::load_little_u64(src);
}

void silkworm_endian_store_big_u16(uint8_t* dst, uint16_t value) {
    silkworm::endian::store_big_u16(dst, value);
}

void silkworm_endian_store_big_u32(uint8_t* dst, uint32_t value) {
    silkworm::endian::store_big_u32(dst, value);
}

void silkworm_endian_store_big_u64(uint8_t* dst, uint64_t value) {
    silkworm::endian::store_big_u64(dst, value);
}

void silkworm_endian_store_little_u16(uint8_t* dst, uint16_t value) {
    silkworm::endian::store_little_u16(dst, value);
}

void silkworm_endian_store_little_u32(uint8_t* dst, uint32_t value) {
    silkworm::endian::store_little_u32(dst, value);
}

void silkworm_endian_store_little_u64(uint8_t* dst, uint64_t value) {
    silkworm::endian::store_little_u64(dst, value);
}

silkworm_ByteView silkworm_endian_to_big_compact_u64(uint64_t value) {
    silkworm::ByteView result = silkworm::endian::to_big_compact(value);
    return {result.data(), result.length()};
}

silkworm_ByteView silkworm_endian_to_big_compact_u256(const uint8_t* value) {
    intx::uint256 uint256_value;
    std::memcpy(&uint256_value, value, sizeof(intx::uint256));
    silkworm::ByteView result = silkworm::endian::to_big_compact(uint256_value);
    return {result.data(), result.length()};
}

silkworm_DecodingResult silkworm_endian_from_big_compact_u64(const silkworm_ByteView* data, uint64_t* out) {
    silkworm::ByteView cpp_data{data->data, data->length};
    auto result = silkworm::endian::from_big_compact(cpp_data, *out);
    silkworm_DecodingResult c_result;
    if (result) {
        c_result.has_error = 0;
    } else {
        c_result.has_error = 1;
        c_result.error = static_cast<silkworm_DecodingError>(result.error());
    }
    return c_result;
}

silkworm_DecodingResult silkworm_endian_from_big_compact_u256(const silkworm_ByteView* data, uint8_t* out) {
    silkworm::ByteView cpp_data{data->data, data->length};
    intx::uint256 uint256_value;
    auto result = silkworm::endian::from_big_compact(cpp_data, uint256_value);
    silkworm_DecodingResult c_result;
    if (result) {
        c_result.has_error = 0;
        std::memcpy(out, &uint256_value, sizeof(intx::uint256));
    } else {
        c_result.has_error = 1;
        c_result.error = static_cast<silkworm_DecodingError>(result.error());
    }
    return c_result;
}